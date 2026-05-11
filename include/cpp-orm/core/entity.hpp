#pragma once
#include "column.hpp"
#include "extractors.hpp"
#include "traits.hpp"
#include <sstream>
#include <vector>
#include <iostream>

namespace orm {

    template<typename Derived, typename... EntityTraits>
    class EntityBase {
    private:
        static constexpr bool is_debug() {
#ifdef ORM_DEBUG
            return true;
#else
            return false;
#endif
        }

        static void debug_print(const std::string& title, const std::string& msg) {
            if constexpr (is_debug()) {
                std::cerr << "[ORM] -> " << title << " : " << msg << '\n';
            }
        }

    public:
        static constexpr const char* table_name() {
            return extract_entity_name<EntityTraits...>::value;
        }
        
        static std::string create_table_sql() {
            std::ostringstream sql;
            sql << "CREATE TABLE IF NOT EXISTS " << table_name() << " (\n";
            
            std::vector<std::string> columns;
            
            Derived temp{};
            temp.for_each_column([&](auto& col) {
                using ColType = std::decay_t<decltype(col)>;
                columns.push_back("  " + ColType::column_sql());
            });
            
            for (size_t i = 0; i < columns.size(); ++i) {
                sql << columns[i];
                if (i < columns.size() - 1) sql << ",";
                sql << "\n";
            }
            
            sql << ");";
            debug_print("CREATE TABLE", sql.str());
            return sql.str();
        }
        
        template<typename Connection>
        void insert(Connection& conn) {
            auto& self = static_cast<Derived&>(*this);
            
            std::ostringstream sql;
            sql << "INSERT INTO " << table_name() << " (";
            
            std::vector<std::string> col_names;
            std::vector<std::string> col_values;
            
            self.for_each_column([&](auto& col) {
                using ColType = std::decay_t<decltype(col)>;
                
                if constexpr (ColType::is_primary_key() 
                        && has_trait<AutoIncrement, typename ColType::traits_tuple>::value) {
                    return;
                }
                
                col_names.push_back(ColType::column_name());
                col_values.push_back(to_sql_string(col.value));
            });
            
            sql << join(col_names, ", ") << ") VALUES (";
            sql << join(col_values, ", ") << ");";

            debug_print("INSERT", sql.str());
            
            // conn.execute(sql.str());
        }
        
        template<typename Connection>
        void update(Connection& conn) {
            auto& self = static_cast<Derived&>(*this);
            
            std::ostringstream sql;
            sql << "UPDATE " << table_name() << " SET ";
            
            std::vector<std::string> set_clauses;
            std::string pk_condition;
            
            self.for_each_column([&](auto& col) {
                using ColType = std::decay_t<decltype(col)>;
                
                if constexpr (ColType::is_primary_key()) {
                    pk_condition = std::string(ColType::column_name()) 
                                + " = " + to_sql_string(col.value);
                } else {
                    set_clauses.push_back(
                        std::string(ColType::column_name()) 
                        + " = " + to_sql_string(col.value)
                    );
                }
            });
            
            sql << join(set_clauses, ", ");
            sql << " WHERE " << pk_condition << ";";

            debug_print("UPDATE", sql.str());
            
            // conn.execute(sql.str());
        }
        
        template<typename Connection>
        static std::vector<Derived> find(Connection& conn, 
                                        const std::string& where = "") {
            std::ostringstream sql;
            sql << "SELECT ";
            
            std::vector<std::string> select_cols;
            Derived temp{};
            temp.for_each_column([&](auto& col) {
                using ColType = std::decay_t<decltype(col)>;
                if constexpr (ColType::is_selectable()) {
                    select_cols.push_back(ColType::column_name());
                }
            });
            
            sql << join(select_cols, ", ");
            sql << " FROM " << table_name();
            
            if (!where.empty()) {
                sql << " WHERE " << where;
            }
            sql << ";";

            debug_print("FIND", sql.str());
            
            return conn.template query<Derived>(sql.str());
        }

        template<typename Connection>
        void remove(Connection& conn) {
            auto& self = static_cast<Derived&>(*this);
            std::ostringstream sql;
            sql << "DELETE FROM " << table_name() 
                << " WHERE " << primary_key_condition(self) << ";";

            debug_print("REMOVE", sql.str());

            // conn.execute(sql.str());
        }
        
    private:
        static std::string join(const std::vector<std::string>& vec, const std::string& delimiter) {
            std::ostringstream result;
            for (size_t i = 0; i < vec.size(); ++i) {
                if (i > 0) result << delimiter;
                result << vec[i];
            }

            debug_print("JOIN", result.str());

            return result.str();
        }
        
        template<typename Self>
        static std::string primary_key_condition(Self& self) {
            std::vector<std::string> conditions;
            self.for_each_column([&](auto& col) {
                using ColType = std::decay_t<decltype(col)>;
                if constexpr (ColType::is_primary_key()) {
                    conditions.push_back(
                        std::string(ColType::column_name()) + " = " 
                        + to_sql_string(col.value)
                    );
                }
            });

            auto res = join(conditions, " AND ");

            return res;
        }
        
        template<typename T>
        static std::string to_sql_string(const T& val) {
            if constexpr (std::is_same_v<T, std::string>) {
                return "'" + val + "'";
            } else if constexpr (std::is_same_v<T, bool>) {
                return val ? "TRUE" : "FALSE";
            } else if constexpr (std::is_arithmetic_v<T>) {
                return std::to_string(val);
            } else {
                return "?";
            }
        }
    };
}
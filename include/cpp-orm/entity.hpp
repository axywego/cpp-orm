#pragma once
#include "column.hpp"
#include <sstream>
#include <vector>

namespace orm {

    #define ORM_FIELD(field_name) \
        static constexpr auto& get_##field_name() { return field_name; }

    template<typename Derived, typename... EntityTraits>
    class EntityBase {
    public:
        static constexpr const char* table_name() {
            return extract_entity_name<EntityTraits...>::value;
        }
        
        static std::string create_table_sql() {
            std::ostringstream sql;
            sql << "CREATE TABLE IF NOT EXISTS " << table_name() << " (\n";
            
            std::vector<std::string> columns;
            static_cast<Derived*>(nullptr)->for_each_column([&](auto& col) {
                using ColType = std::decay_t<decltype(col)>;
                columns.push_back("  " + ColType::column_sql());
            });
            
            for (size_t i = 0; i < columns.size(); ++i) {
                sql << columns[i];
                if (i < columns.size() - 1) sql << ",";
                sql << "\n";
            }
            
            sql << ");";
            return sql.str();
        }
        
        template<typename Connection>
        void save(Connection& conn) {
            auto& self = static_cast<Derived&>(*this);
            
            std::ostringstream sql;
            sql << "INSERT INTO " << table_name() << " (";
            
            std::vector<std::string> col_names;
            std::vector<std::string> values;
            
            self.for_each_column([&](auto& col) {
                using ColType = std::decay_t<decltype(col)>;
                
                if constexpr (ColType::is_primary_key() 
                        && has_trait<AutoIncrement, decltype(col.traits)>::value) {
                    return;
                }
                
                col_names.push_back(ColType::column_name());
                values.push_back(conn.escape(*col));
            });
            
            sql << join(col_names, ", ") << ") VALUES (";
            sql << join(values, ", ") << ");";
            
            conn.execute(sql.str());
        }
        
        template<typename Connection>
        static std::vector<Derived> find(Connection& conn, 
                                        const std::string& where = "") {
            std::ostringstream sql;
            sql << "SELECT ";
            
            std::vector<std::string> select_cols;
            static_cast<Derived*>(nullptr)->for_each_column([&](auto& col) {
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
            
            return conn.template query<Derived>(sql.str());
        }
        
        template<typename Connection>
        void soft_delete(Connection& conn) {
            auto& self = static_cast<Derived&>(*this);
            std::ostringstream sql;
            sql << "DELETE FROM " << table_name() 
                << " WHERE " << primary_key_condition(self);
            conn.execute(sql.str());
        }
        
    private:
        static std::string join(const std::vector<std::string>& vec, 
                            const std::string& delimiter) {
            std::ostringstream result;
            for (size_t i = 0; i < vec.size(); ++i) {
                result << vec[i];
                if (i < vec.size() - 1) result << delimiter;
            }
            return result.str();
        }
        
        template<typename Self>
        static std::string primary_key_condition(Self& self) {
            std::string condition;
            self.for_each_column([&](auto& col) {
                using ColType = std::decay_t<decltype(col)>;
                if constexpr (ColType::is_primary_key()) {
                    if (!condition.empty()) condition += " AND ";
                    condition += std::string(ColType::column_name()) + " = " 
                            + std::to_string(*col);
                }
            });
            return condition;
        }
    };

}
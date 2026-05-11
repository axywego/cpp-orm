#pragma once
#include <memory>
#include <stdexcept>
#include <pqxx/pqxx>

namespace orm {

    class Database {
    private:
        static inline std::unique_ptr<pqxx::connection> conn = nullptr;

    public:
        static void connect(const std::string& conn_str) {
            conn = std::make_unique<pqxx::connection>(conn_str);
        }

        static void disconnect() {
            conn.reset();
        }

        void execute(const std::string& sql) {
            if (!conn) {
                throw std::runtime_error("Not connected to database!");
            }
            pqxx::work txn(*conn);
            txn.exec(sql);
            txn.commit();
        }

        template<typename Entity>
        std::vector<Entity> query(const std::string& sql) {
            if (!conn) {
                throw std::runtime_error("Not connected to database!");
            }
            pqxx::work txn(*conn);
            pqxx::result res = txn.exec(sql);
            
            std::vector<Entity> entities;
            
            for (const auto& row : res) {
                Entity entity;
                int col_index = 0;
                
                entity.for_each_column([&](auto& col) {
                    using ColType = std::decay_t<decltype(col)>;
                    
                    if constexpr (ColType::is_selectable()) {
                        if (!row[col_index].is_null()) {
                            set_value(col.value, row[col_index]);
                        }
                        col_index++;
                    }
                });
                
                entities.push_back(std::move(entity));
            }
            
            return entities;
        }

    private:
        static void set_value(int& field, const pqxx::field& val) {
            field = val.as<int>();
        }
        
        static void set_value(long& field, const pqxx::field& val) {
            field = val.as<long>();
        }
        
        static void set_value(std::string& field, const pqxx::field& val) {
            field = val.as<std::string>();
        }
        
        static void set_value(double& field, const pqxx::field& val) {
            field = val.as<double>();
        }
        
        static void set_value(bool& field, const pqxx::field& val) {
            field = val.as<bool>();
        }

        static void set_value(float& field, const pqxx::field& val) {
            field = val.as<float>();
        }
    };

}
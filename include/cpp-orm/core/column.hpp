#pragma once

#include "traits.hpp"
#include "extractors.hpp"
#include <string>
#include <utility>
#include <tuple>
#include <type_traits>

namespace orm {

    template<typename T, typename... Traits>
    class Column {
    public:
        using value_type = T;
        using traits_tuple = std::tuple<Traits...>; 
        
        T value{};

        Column() : value(get_default()) {}
        Column(T val) : value(std::move(val)) {}

        Column& operator=(const T& val) { value = val; return *this; }
        operator T&() { return value; }
        operator const T&() const { return value; }
                
        T& operator*() { return value; }
        const T& operator*() const { return value; }
        T* operator->() { return &value; }
        const T* operator->() const { return &value; }
        
        static constexpr bool is_nullable() {
            return !has_trait<NotNull, Traits...>::value;
        }

        static constexpr bool is_unique() {
            return has_trait<Unique, Traits...>::value;
        }

        static constexpr bool is_primary_key() {
            return has_trait<PrimaryKey, Traits...>::value;
        }

        static constexpr bool is_selectable() {
            return extract_select<Traits...>::value;
        }

        static constexpr T get_default() {
            if constexpr (extract_default<Traits...>::has_value) {
                return extract_default<Traits...>::value;
            }
            return T{};
        }

        static constexpr const char* column_name() {
            if constexpr (extract_column_name<Traits...>::has_value) {
                return extract_column_name<Traits...>::value;
            }
            return "unnamed_column";
        }

        static constexpr const char* comment() {
            if constexpr (extract_comment<Traits...>::has_value) {
                return extract_comment<Traits...>::value;
            }
            return "";
        }

        static constexpr const char* sql_type() {
            if constexpr (std::is_same_v<T, int>) {
                if constexpr (has_trait<PrimaryKey, Traits...>::value 
                        && has_trait<AutoIncrement, Traits...>::value) {
                    return "SERIAL";
                } 
                return "INTEGER";
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                if constexpr (has_trait<PrimaryKey, Traits...>::value 
                        && has_trait<Uuid, Traits...>::value) {
                    return "UUID";
                }
                if constexpr (extract_varchar<Traits...>::has_value) {
                    return "VARCHAR";
                }
                return "TEXT";
            }
            else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>) {
                return "DOUBLE PRECISION";
            }
            else if constexpr (std::is_same_v<T, bool>) {
                return "BOOLEAN";
            }
            return "UNKNOWN";
        }

        static std::string column_sql() {
            std::string sql;
            sql += column_name();
            sql += " ";
            sql += sql_type();

            if constexpr (extract_varchar<Traits...>::has_value) {
                sql += "(" + std::to_string(extract_varchar<Traits...>::value) + ")";
            }

            if constexpr (extract_decimal<Traits...>::has_value) {
                sql += "(" + std::to_string(extract_decimal<Traits...>::precision) 
                    + "," + std::to_string(extract_decimal<Traits...>::scale) + ")";
            }

            if constexpr (!is_nullable()) {
                sql += " NOT NULL";
            }

            if constexpr (is_unique()) {
                sql += " UNIQUE";
            }

            if constexpr (extract_default<Traits...>::has_value) {
                sql += " DEFAULT " + format_sql_value(get_default());
            }
            
            return sql;
        }

    private:
        template<typename U>
        static std::string format_sql_value(U val) {
            if constexpr (std::is_same_v<U, std::string>) {
                return "'" + val + "'";
            }
            else if constexpr (std::is_same_v<U, bool>) {
                return val ? "TRUE" : "FALSE";
            }
            else if constexpr (std::is_arithmetic_v<U>) {
                return std::to_string(val);
            }
            else {
                return "?";
            }
        }
    };

}
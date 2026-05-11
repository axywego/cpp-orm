#pragma once

#include <cstddef>
#include <type_traits>
#include <array>
#include <string_view>

namespace orm {

    template<typename T>
    struct ColumnType {
        using value_type = T;
    };

    template<size_t N>
    struct Varchar {
        static constexpr size_t length = N;
    };

    template<int P, int S>
    struct Decimal {
        static constexpr int precision = P;
        static constexpr int scale = S;
    };

    struct NotNull {};
    struct Nullable {};
    struct Unique {};
    struct PrimaryKey {};
    struct AutoIncrement {};
    struct Uuid {};
    struct Version {};

    template<bool V>
    struct Select {
        static constexpr bool visible = V;
    };

    template<auto V>
    struct Default {
        static constexpr auto value = V;
    };

    template<std::string_view S>
    struct Comment {
        static constexpr std::string_view text = S;
    };

    template<std::string_view S>
    struct ColumnName {
        static constexpr std::string_view name = S;
    };

    template<typename E, std::string_view S>
    struct EnumValue {
        using enum_type = E;
        static constexpr std::string_view string = S;
    };

    template<std::string_view Name>
    struct Entity {
        static constexpr std::string_view table_name = Name;
    };

    template<std::string_view... Columns>
    struct Index {
        static constexpr auto columns = std::array{Columns...};
    };

    template<typename T>
    struct OneToMany {
        using target_type = T;
    };

    template<typename T>
    struct ManyToOne {
        using target_type = T;
    };

    template<typename T>
    struct OneToOne {
        using target_type = T;
    };

}
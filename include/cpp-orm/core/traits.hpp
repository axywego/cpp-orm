#pragma once

#include <cstddef>
#include <type_traits>
#include <array>

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

    template<const char* S>
    struct Comment {
        static constexpr const char* text = S;
    };

    template<const char* S>
    struct ColumnName {
        static constexpr const char* name = S;
    };

    template<typename E, const char* S>
    struct EnumValue {
        using enum_type = E;
        static constexpr const char* string = S;
    };

    template<const char* Name>
    struct Entity {
        static constexpr const char* table_name = Name;
    };

    template<const char*... Columns>
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
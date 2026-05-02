#pragma once

#include <cstdlib>
#include <type_traits>

namespace orm {
    template<typename T>
    struct ColumnType {
        using value_type = T;
    };

    template<size_t N>
    struct Varchar {
        static constexpr size_t length = N;
    };

    struct NotNull{};

    struct Nullable{};

    struct Unique{};

    template<bool V>
    struct Select {
        static constexpr bool visible = V;
    };

    template<auto V>
    struct Default {
        static constexpr auto value = V;
    };

    struct PrimaryKey{};
    
    struct AutoIncrement{};

    struct Uuid {};

    template<const char* Name>
    struct Entity {
        static constexpr const char* table_name = Name;
    };

    template<typename T>
    struct OneToMany {
        using target_type = T;
    };

    template<typename T>
    struct OneToOne {
        using target_type = T;
    };
}
#pragma once

#include "traits.hpp"
#include <type_traits>
#include <tuple>
#include <array>

namespace orm {

    template<typename Trait, typename... Traits>
    struct has_trait : std::disjunction<std::is_same<Trait, Traits>...> {};

    template<typename... Traits>
    struct extract_default {
        static constexpr bool has_value = false;
    };
    template<auto V, typename... Rest>
    struct extract_default<Default<V>, Rest...> {
        static constexpr auto value = V;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_default<T, Rest...> : extract_default<Rest...> {};

    template<typename... Traits>
    struct extract_varchar {
        static constexpr size_t value = 0;
        static constexpr bool has_value = false;
    };
    template<size_t N, typename... Rest>
    struct extract_varchar<Varchar<N>, Rest...> {
        static constexpr size_t value = N;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_varchar<T, Rest...> : extract_varchar<Rest...> {};

    template<typename... Traits>
    struct extract_decimal {
        static constexpr int precision = 0;
        static constexpr int scale = 0;
        static constexpr bool has_value = false;
    };
    template<int P, int S, typename... Rest>
    struct extract_decimal<Decimal<P, S>, Rest...> {
        static constexpr int precision = P;
        static constexpr int scale = S;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_decimal<T, Rest...> : extract_decimal<Rest...> {};

    template<typename... Traits>
    struct extract_select {
        static constexpr bool value = true;
    };
    template<bool V, typename... Rest>
    struct extract_select<Select<V>, Rest...> {
        static constexpr bool value = V;
    };
    template<typename T, typename... Rest>
    struct extract_select<T, Rest...> : extract_select<Rest...> {};

    template<typename... Traits>
    struct extract_column_name {
        static constexpr const char* value = "";
        static constexpr bool has_value = false;
    };
    template<const char* S, typename... Rest>
    struct extract_column_name<ColumnName<S>, Rest...> {
        static constexpr const char* value = S;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_column_name<T, Rest...> : extract_column_name<Rest...> {};

    template<typename... Traits>
    struct extract_comment {
        static constexpr const char* value = "";
        static constexpr bool has_value = false;
    };
    template<const char* S, typename... Rest>
    struct extract_comment<Comment<S>, Rest...> {
        static constexpr const char* value = S;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_comment<T, Rest...> : extract_comment<Rest...> {};

    template<typename... Traits>
    struct extract_entity_name {
        static constexpr const char* value = "";
        static constexpr bool has_value = false;
    };
    template<const char* S, typename... Rest>
    struct extract_entity_name<Entity<S>, Rest...> {
        static constexpr const char* value = S;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_entity_name<T, Rest...> : extract_entity_name<Rest...> {};

    template<typename... Traits>
    struct extract_enum_values {
        static constexpr auto values = std::tuple<>{};
        static constexpr bool has_value = false;
    };
    template<typename E, const char* S, typename... Rest>
    struct extract_enum_values<EnumValue<E, S>, Rest...> {
        static constexpr bool has_value = true;
        static constexpr auto values = std::tuple_cat(
            std::make_tuple(EnumValue<E, S>{}),
            extract_enum_values<Rest...>::values
        );
    };
    template<typename T, typename... Rest>
    struct extract_enum_values<T, Rest...> : extract_enum_values<Rest...> {};

    template<typename... Traits>
    struct extract_indexes {
        using type = std::tuple<>;
        static constexpr bool has_value = false;
    };
    template<const char*... Columns, typename... Rest>
    struct extract_indexes<Index<Columns...>, Rest...> {
        static constexpr bool has_value = true;
        using type = decltype(std::tuple_cat(
            std::declval<std::tuple<std::array<const char*, sizeof...(Columns)>>>(),
            std::declval<typename extract_indexes<Rest...>::type>()
        ));
    };
    template<typename T, typename... Rest>
    struct extract_indexes<T, Rest...> : extract_indexes<Rest...> {};

    template<typename... Traits>
    struct extract_one_to_many {
        using type = void;
        static constexpr bool has_value = false;
    };
    template<typename T, typename... Rest>
    struct extract_one_to_many<OneToMany<T>, Rest...> {
        using type = T;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_one_to_many<T, Rest...> : extract_one_to_many<Rest...> {};

    template<typename... Traits>
    struct extract_many_to_one {
        using type = void;
        static constexpr bool has_value = false;
    };
    template<typename T, typename... Rest>
    struct extract_many_to_one<ManyToOne<T>, Rest...> {
        using type = T;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_many_to_one<T, Rest...> : extract_many_to_one<Rest...> {};

    template<typename... Traits>
    struct extract_one_to_one {
        using type = void;
        static constexpr bool has_value = false;
    };
    template<typename T, typename... Rest>
    struct extract_one_to_one<OneToOne<T>, Rest...> {
        using type = T;
        static constexpr bool has_value = true;
    };
    template<typename T, typename... Rest>
    struct extract_one_to_one<T, Rest...> : extract_one_to_one<Rest...> {};

}
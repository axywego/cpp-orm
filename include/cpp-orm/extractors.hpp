#pragma once

#include "traits.hpp"

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
    struct extract_entity_name {
        static constexpr const char* value = "";
    };
    template<const char* S, typename... Rest>
    struct extract_entity_name<Entity<S>, Rest...> {
        static constexpr const char* value = S;
    };
    template<typename T, typename... Rest>
    struct extract_entity_name<T, Rest...> : extract_entity_name<Rest...> {};


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
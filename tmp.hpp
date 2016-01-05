//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains Template Metaprogramming (TMP) utilities
 */

#ifndef CPP_UTILS_TMP_HPP
#define CPP_UTILS_TMP_HPP

#include <tuple>
#include <utility>

#include "tmp/tmp.hpp"
#include "tmp/sfinae.hpp"
#include "tmp/variadic.hpp"

namespace cpp {

namespace tmp_detail {

template<std::size_t I, std::size_t S, typename F, typename... T>
struct is_homogeneous_helper {
    template<std::size_t I1, std::size_t S1, typename Enable = void>
    struct helper_int : and_helper<std::is_same<F, nth_type_t<I1, T...>>::value, is_homogeneous_helper<I1+1, S1, F, T...>::value> {};

    template<std::size_t I1, std::size_t S1>
    struct helper_int<I1, S1, std::enable_if_t<I1 == S1>> : std::is_same<F, nth_type_t<I1, T...>> {};

    static constexpr const auto value = helper_int<I, S>::value;
};

} //end of namespace tmp_detail

template<int I, typename Tuple, typename Functor>
struct for_each_tuple_t_impl {
    static void for_each(Functor&& func){
        std::forward<Functor>(func).template operator()<typename std::tuple_element<I, Tuple>::type>();
        for_each_tuple_t_impl<I - 1, Tuple, Functor>::for_each(std::forward<Functor>(func));
    }
};

template<typename Tuple, typename Functor>
struct for_each_tuple_t_impl<0, Tuple, Functor> {
    static void for_each(Functor&& func){
        std::forward<Functor>(func).template operator()<typename std::tuple_element<0, Tuple>::type>();
    }
};

template<typename Tuple, typename Functor>
struct for_each_tuple_t_impl<-1, Tuple, Functor> {
    static void for_each(Functor&& /*func*/){
        //Nothing to be done
    }
};

template<typename Tuple, typename Functor>
void for_each_tuple_t(Functor&& func){
    for_each_tuple_t_impl<static_cast<int>(std::tuple_size<Tuple>::value) - 1, Tuple, Functor>::for_each(std::forward<Functor>(func));
}

template<template<typename...> class TT, typename T>
struct is_specialization_of : std::false_type {};

template<template<typename...> class TT, typename... Args>
struct is_specialization_of<TT, TT<Args...>> : std::true_type {};

template<typename V, typename F, typename... S>
struct all_convertible_to : bool_constant_c<and_c<all_convertible_to<V, F>, all_convertible_to<V, S...>>> {};

template<typename V, typename F>
struct all_convertible_to<V, F> : bool_constant_c<std::is_convertible<F, V>> {};

template<typename F, typename... T>
struct is_homogeneous : bool_constant_c<tmp_detail::is_homogeneous_helper<0, sizeof...(T)-1, F, T...>> {};

template<typename F>
struct is_homogeneous <F> : std::true_type {};

template<typename... T>
struct is_sub_homogeneous;

template<>
struct is_sub_homogeneous<> : std::false_type {};

template<typename T>
struct is_sub_homogeneous<T> : std::false_type {};

template<typename T1, typename T2>
struct is_sub_homogeneous<T1, T2> : bool_constant_c<not_c<std::is_same<T1, T2>>> {};

template<typename T1, typename T2, typename T3, typename... T>
struct is_sub_homogeneous<T1, T2, T3, T...> : bool_constant_c<
    and_c<
        std::is_same<T1, T2>,
        is_sub_homogeneous<T2, T3, T...>
    >> {};

template<typename F, std::size_t I1, std::size_t... I, typename... T, enable_if_u<(sizeof...(I) == 0)> = detail::dummy>
void for_each_in_subset(F&& f, const std::index_sequence<I1, I...>& /*i*/, T&&... args){
    f(std::forward<nth_type_t<I1, T...>>(nth_value<I1>(args...)));
}

template<typename F, std::size_t I1, std::size_t... I, typename... T, enable_if_u<(sizeof...(I) > 0)> = detail::dummy>
void for_each_in_subset(F&& f, const std::index_sequence<I1, I...>& /*i*/, T&&... args){
    f(std::forward<nth_type_t<I1, T...>>(nth_value<I1>(args...)));
    for_each_in_subset(f, std::index_sequence<I...>(), std::forward<T>(args)...);
}

template<typename F, typename... T>
void for_each_in(F&& f, T&&... args){
    for_each_in_subset(f, std::make_index_sequence<sizeof...(T)>(), std::forward<T>(args)...);
}

template<typename T1, typename... T>
struct variadic_contains;

template<typename T1, typename T2, typename... T>
struct variadic_contains<T1, T2, T...> : bool_constant_c<or_c<std::is_same<T1, T2>, variadic_contains<T1, T...>>> {};

template<typename T1>
struct variadic_contains<T1> : std::false_type {};

/*!
 * \brief A compile-time type list.
 */
template<typename... T>
struct type_list {
    /*!
     * \brief Indicates if the list contains the given type
     * \tparam V The type to search in the list
     * \return true if the type is in the list, false otherwise.
     */
    template<typename V>
    static constexpr bool contains(){
        return variadic_contains<V, T...>::value;
    }
};

} //end of namespace cpp

#endif //CPP_UTILS_TMP_HPP

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

#include "assert.hpp"

#include "tmp/tmp.hpp"
#include "tmp/sfinae.hpp"
#include "tmp/variadic.hpp"

namespace cpp {

namespace tmp_detail {

    /*!
     * \brief Helper Traits to test if all the types from I to N in
     * T are homogeneous
     */
template <std::size_t I, std::size_t S, typename F, typename... T>
struct is_homogeneous_helper {
    /*!
     * \brief Sub helper for recursive instantiations
     */
    template <std::size_t I1, std::size_t S1, typename Enable = void>
    struct helper_int : and_u<std::is_same_v<F, nth_type_t<I1, T...>>, is_homogeneous_helper<I1 + 1, S1, F, T...>::value> {};

    /*!
     * \copydoc helper_int
     */
    template <std::size_t I1, std::size_t S1>
    struct helper_int<I1, S1, std::enable_if_t<I1 == S1>> : std::is_same<F, nth_type_t<I1, T...>> {};

    static constexpr const auto value = helper_int<I, S>::value; ///< Indicates if the sequence of types is homogeneous
};

} //end of namespace tmp_detail

/*!
 * \brief Implementation of helper for for_each_tuple_t
 */
template <int I, typename Tuple, typename Functor>
struct for_each_tuple_t_impl {
    /*!
     * \brief Apply the functof for each elements in the tuple, from I
     */
    static void for_each(Functor&& func) {
        std::forward<Functor>(func).template operator()<typename std::tuple_element<I, Tuple>::type>();
        for_each_tuple_t_impl<I - 1, Tuple, Functor>::for_each(std::forward<Functor>(func));
    }
};

/*!
 * \copydoc for_each_tuple_t_impl
 */
template <typename Tuple, typename Functor>
struct for_each_tuple_t_impl<0, Tuple, Functor> {
    /*!
     * \brief Apply the functof for each elements in the tuple, from I
     */
    static void for_each(Functor&& func) {
        std::forward<Functor>(func).template operator()<typename std::tuple_element<0, Tuple>::type>();
    }
};

/*!
 * \copydoc for_each_tuple_t_impl
 */
template <typename Tuple, typename Functor>
struct for_each_tuple_t_impl<-1, Tuple, Functor> {
    /*!
     * \brief End of the the recursion
     */
    static void for_each(Functor&& /*func*/) {
        //Nothing to be done
    }
};

/*!
 * \brief Call the given functor for each type in the tuple
 * \tparam Tuple The tuple type
 * \param func The functor to call
 */
template <typename Tuple, typename Functor>
void for_each_tuple_t(Functor&& func) {
    for_each_tuple_t_impl<static_cast<int>(std::tuple_size<Tuple>::value) - 1, Tuple, Functor>::for_each(std::forward<Functor>(func));
}

/*!
 * \brief Traits to test if a type is a specialization of a template
 * \tparam TT The template type
 * \tparam T The type to test
 */
template <template <typename...> class TT, typename T>
struct is_specialization_of : std::false_type {};

/*!
 * \copydoc is_specialization_of
 */
template <template <typename...> class TT, typename... Args>
struct is_specialization_of<TT, TT<Args...>> : std::true_type {};

/*!
 * \brief Traits to test if all the given types are convertible to V
 * \tparam V The target type
 * \tparam F The first type to test
 * \tparam S The types to test
 */
template <typename V, typename... S>
using all_convertible_to = std::bool_constant<(... && std::is_convertible_v<V, S>)>;

/*!
 * \brief Test is a list of types homogeneous
 * \tparam F The first type
 * \tparam T The types
 */
template <typename F, typename... T>
struct is_homogeneous : bool_constant_c<tmp_detail::is_homogeneous_helper<0, sizeof...(T)-1, F, T...>> {};

/*!
 * \copydoc all_convertible_to
 */
template <typename F>
struct is_homogeneous<F> : std::true_type {};

/*
 * \brief Test if a list of types is sub-homogeneous
 *
 * A sub-homogeneous list of types is a list where the N-1 first types are the
 * same and the last one is different
 */
template <typename... T>
struct is_sub_homogeneous;

/*!
 * \copydoc is_sub_homogeneous
 */
template <>
struct is_sub_homogeneous<> : std::false_type {};

/*!
 * \copydoc is_sub_homogeneous
 */
template <typename T>
struct is_sub_homogeneous<T> : std::false_type {};

/*!
 * \copydoc is_sub_homogeneous
 */
template <typename T1, typename T2>
struct is_sub_homogeneous<T1, T2> : bool_constant_c<not_c<std::is_same<T1, T2>>> {};

/*!
 * \copydoc is_sub_homogeneous
 */
template <typename T1, typename T2, typename T3, typename... T>
struct is_sub_homogeneous<T1, T2, T3, T...> : bool_constant_c<
                                                  and_c<
                                                      std::is_same<T1, T2>,
                                                      is_sub_homogeneous<T2, T3, T...>>> {};

/*!
 * \brief Apply the given function to each element of the variadic
 * packs whose position is present in the sequence
 * \param f The functor
 * \param i The index sequence
 * \param args The arguments
 */
template <typename F, std::size_t I1, std::size_t... I, typename... T, enable_if_u<(sizeof...(I) == 0)> = detail::dummy>
void for_each_in_subset(F&& f, const std::index_sequence<I1, I...>& i, T&&... args) {
    f(std::forward<nth_type_t<I1, T...>>(nth_value<I1>(args...)));
    cpp_unused(i);
}

/*!
 * \brief Apply the given function to each element of the variadic
 * packs whose position is present in the sequence
 * \param f The functor
 * \param i The index sequence
 * \param args The arguments
 */
template <typename F, std::size_t I1, std::size_t... I, typename... T, enable_if_u<(sizeof...(I) > 0)> = detail::dummy>
void for_each_in_subset(F&& f, const std::index_sequence<I1, I...>& i, T&&... args) {
    f(std::forward<nth_type_t<I1, T...>>(nth_value<I1>(args...)));
    for_each_in_subset(f, std::index_sequence<I...>(), std::forward<T>(args)...);
    cpp_unused(i);
}

/*!
 * \brief Apply the given function to each element of the variadic pack
 * \param f The functor
 * \param args The arguments
 */
template <typename F, typename... T>
void for_each_in(F&& f, T&&... args) {
    for_each_in_subset(f, std::make_index_sequence<sizeof...(T)>(), std::forward<T>(args)...);
}

/*!
 * \brief Test if the variadic list of types containg the given type
 * \tparam T1 The type to search
 * \tparam T The list of types
 */
template <typename T1, typename... T>
struct variadic_contains : std::bool_constant<(... || std::is_same_v<T1, T>)> {};

/*!
 * \brief A compile-time type list.
 */
template <typename... T>
struct type_list {
    /*!
     * \brief Indicates if the list contains the given type
     * \tparam V The type to search in the list
     * \return true if the type is in the list, false otherwise.
     */
    template <typename V>
    static constexpr bool contains() {
        return variadic_contains<V, T...>::value;
    }
};

/*!
 * \brief Value Traits to test if a type is a specialization of a template
 *
 * \tparam TT The template type
 * \tparam T The type to test
 */
template <template <typename...> class TT, typename T>
constexpr bool is_specialization_of_v = is_specialization_of<TT, T>::value;

/*!
 * \brief Value traits to test if all the given types are convertible to V
 *
 * \tparam V The target type
 * \tparam F The first type to test
 * \tparam S The types to test
 */
template <typename V, typename F, typename... S>
constexpr bool all_convertible_to_v = all_convertible_to<V, F, S...>::value;

/*!
 * \brief Value traits to test if is a list of types homogeneous
 * \tparam F The first type
 * \tparam T The types
 */
template <typename F, typename... T>
constexpr bool is_homogeneous_v = is_homogeneous<F, T...>::value;

/*
 * \brief Value traits to test if a list of types is sub-homogeneous
 *
 * A sub-homogeneous list of types is a list where the N-1 first types are the
 * same and the last one is different
 */
template <typename... T>
constexpr bool is_sub_homogeneous_v = is_sub_homogeneous<T...>::value;

/*!
 * \brief Value traits to test if the variadic list of types containg the given type
 * \tparam T1 The type to search
 * \tparam T The list of types
 */
template <typename T1, typename... T>
constexpr bool variadic_contains_v = variadic_contains<T1, T...>::value;

} //end of namespace cpp

#endif //CPP_UTILS_TMP_HPP

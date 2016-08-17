//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains Template Metaprogramming (TMP) utilities for variadic.
 */

#ifndef CPP_UTILS_TMP_VARIADIC_HPP
#define CPP_UTILS_TMP_VARIADIC_HPP

#include "sfinae.hpp"

namespace cpp {

/*!
 * \brief Traits to get the Nth type from a variadic list of types.
 * \tparam N The index to get
 * \tparam T The list of types
 */
template <std::size_t N, typename... T>
struct nth_type {
    using type = typename std::tuple_element<N, std::tuple<T...>>::type; ///< The result type
};

/*!
 * \brief Traits to get the first type from a variadic list of types.
 * \tparam T The list of types
 */
template <typename... T>
struct first_type {
    using type = typename nth_type<0, T...>::type; ///< The result type
};

/*!
 * \brief Traits to get the last type from a variadic list of types.
 * \tparam T The list of types
 */
template <typename... T>
struct last_type {
    using type = typename nth_type<sizeof...(T)-1, T...>::type; ///< The result type
};

/*!
 * \brief Helper traits to get the Nth type from a variadic type list
 * \tparam N The index to get
 * \tparam T The list of types
 */
template <std::size_t N, typename... T>
using nth_type_t = typename nth_type<N, T...>::type;

/*!
 * \brief Helper traits to get the last type from a variadic type list
 * \tparam T The list of types
 */
template <typename... T>
using first_type_t = typename first_type<T...>::type;

/*!
 * \brief Helper traits to get the last type from a variadic type list
 * \tparam T The list of types
 */
template <typename... T>
using last_type_t = typename last_type<T...>::type;

/*!
 * \brief Extract the Ith value from the variadic list
 */
template <int I, typename T1, typename... T, cpp_enable_if(I == 0)>
auto nth_value(T1&& t, T&&... /*args*/) -> decltype(std::forward<T1>(t)) {
    return std::forward<T1>(t);
}

/*!
 * \brief Extract the Ith value from the variadic list
 */
template <int I, typename T1, typename... T, cpp_enable_if((I > 0))>
auto nth_value(T1&& /*t*/, T&&... args)
    -> decltype(std::forward<nth_type_t<I, T1, T...>>(std::declval<nth_type_t<I, T1, T...>>())) {
    return std::forward<nth_type_t<I, T1, T...>>(nth_value<I - 1>((std::forward<T>(args))...));
}

/*!
 * \brief Returns the last value from the variadic pack
 * \param args The variadic pack of arguments
 * \return The last value from the variadic pack
 */
template <typename... T>
auto last_value(T&&... args) {
    return std::forward<last_type_t<T...>>(nth_value<sizeof...(T)-1>(args...));
}

/*!
 * \brief Returns the first value from the variadic pack
 * \param args The variadic pack of arguments
 * \return The first value from the variadic pack
 */
template <typename... T>
auto first_value(T&&... args) {
    return std::forward<first_type_t<T...>>(nth_value<0>(args...));
}

} //end of namespace cpp

#endif //CPP_UTILS_TMP_HPP

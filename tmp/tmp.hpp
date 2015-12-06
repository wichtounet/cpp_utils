//=======================================================================
// Copyright (c) 2013-2014-2015 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains Template Metaprogramming (TMP) utilities
 */

#ifndef CPP_UTILS_TMP_TMP_HPP
#define CPP_UTILS_TMP_TMP_HPP

namespace cpp {

namespace tmp_detail {

template<bool H, bool... T>
struct and_helper : std::integral_constant<bool, H && and_helper<T...>::value> {};

template<bool H>
struct and_helper<H> : std::integral_constant<bool, H> {};

template<bool H, bool... T>
struct or_helper : std::integral_constant<bool, H || or_helper<T...>::value> {};

template<bool H>
struct or_helper<H> : std::integral_constant<bool, H> {};

} //end of namespace tmp_detail

/*!
 * \brief TMP helper implementing the NOT boolean operator taking a boolean constant value.
 *
 * Represents an integral constant with the negated value of the given boolean constant.
 *
 * \tparam B The boolean value to negate
 */
template<bool B>
struct not_u : std::integral_constant<bool, !B> {};

/*!
 * \brief TMP helper implementing the NOT boolean operator taking a TMP class.
 *
 * Represents an integral constant with the negated value of the given TMP class.
 *
 * \tparam C The TMP class whose value to negate.
 */
template<typename C>
struct not_c : std::integral_constant<bool, !C::value> {};

/*!
 * \brief TMP helper implementing the AND boolean operator of given boolean values.
 *
 * Represents an integral constant with the ANDed value of the given boolean values.
 *
 * \tparam C The sequence of boolean values to AND.
 */
template<bool... C>
using and_u = tmp_detail::and_helper<C...>;

/*!
 * \brief TMP helper implementing the AND boolean operator taking a sequence TMP class.
 *
 * Represents an integral constant with the ANDed value of the given TMP classes.
 *
 * \tparam C The sequence of TMP classes whose value to AND.
 */
template<typename... C>
using and_c = tmp_detail::and_helper<C::value...>;

/*!
 * \brief TMP helper implementing the OR boolean operator of given boolean values.
 *
 * Represents an integral constant with the ANDed value of the given boolean values.
 *
 * \tparam C The sequence of boolean values to OR.
 */
template<bool... C>
using or_u = tmp_detail::or_helper<C...>;

/*!
 * \brief TMP helper implementing the OR boolean operator taking a sequence TMP class.
 *
 * Represents an integral constant with the ANDed value of the given TMP classes.
 *
 * \tparam C The sequence of TMP classes whose value to OR.
 */
template<typename... C>
using or_c = tmp_detail::or_helper<C::value...>;

/*!
 * \brief Traits to add const and lvalue reference to T.
 * \tparam T the type to add const and lvalue reference to.
 */
template<typename T>
using add_const_lvalue_t = std::add_lvalue_reference_t<std::add_const_t<T>>;

/*!
 * \brief Traits to add const and rvalue reference to T.
 * \tparam T the type to add const and rvalue reference to.
 */
template<typename T>
using add_const_rvalue_t = std::add_rvalue_reference_t<std::add_const_t<T>>;

//Integral constants

template<typename T, typename C>
using integral_constant_c = std::integral_constant<T, C::value>;

/*!
 * \brief Base class for an integral boolean constant.
 * \tparam B The boolean constant value.
 */
template<bool B>
using bool_constant = std::integral_constant<bool, B>;

/*!
 * \brief Base class for an integral boolean constant.
 * \tparam C The TMP class to extract the boolean value from.
 */
template<typename C>
using bool_constant_c = std::integral_constant<bool, C::value>;

/*!
 * \brief Base class for an integral constant of auto type.
 *
 * The type of the integral constant is automatically deduced from the the value member of the given TMP class.
 *
 * \tparam V The TMP class to extract the value from.
 */
template<typename V>
struct auto_constant : std::integral_constant<decltype(V::value), V::value> {};

template<bool C, typename V1, typename V2>
struct conditional_constant;

template<typename V1, typename V2>
struct conditional_constant<true, V1, V2> : auto_constant<V1> {};

template<typename V1, typename V2>
struct conditional_constant<false, V1, V2> : auto_constant<V2> {};

/*!
 * \brief Base class to define a type constant
 * \tparam T The type constant.
 */
template<typename T>
struct type_constant {
    using type = T;
};

/*!
 * \brief Base class to define a type constant
 * \tparam T The type to extract the type constant from.
 */
template<typename T>
struct type_constant_c {
    using type = typename T::value;
};

template<bool C, typename V1, typename V2>
struct conditional_type_constant;

template<typename V1, typename V2>
struct conditional_type_constant<true, V1, V2> {
    using type = V1;
};

template<typename V1, typename V2>
struct conditional_type_constant<false, V1, V2> {
    using type = V2;
};

template<bool C, typename V1, typename V2>
struct conditional_type_constant_c;

template<typename V1, typename V2>
struct conditional_type_constant_c<true, V1, V2> {
    using type = typename V1::value;
};

template<typename V1, typename V2>
struct conditional_type_constant_c<false, V1, V2> {
    using type = typename V2::value;
};

/*!
 * \brief Base class to define a template type constant
 * \tparam TT The template type constant.
 */
template<typename TT>
struct template_type_constant {
    template<typename T>
    using type = TT;
};

/*!
 * \brief Base class to define a template type constant
 * \tparam TT The type to extract the template type constant from.
 */
template<typename TT>
struct template_type_constant_c {
    template<typename T>
    using type = typename TT::template value<T>;
};

template<bool, template<typename> class V1, template<typename> class V2>
struct conditional_template_type_constant;

template<template<typename> class V1, template<typename> class V2>
struct conditional_template_type_constant<true, V1, V2> {
    template<typename T>
    using type = V1<T>;
};

template<template<typename> class V1, template<typename> class V2>
struct conditional_template_type_constant<false, V1, V2> {
    template<typename T>
    using type = V2<T>;
};

template<bool C, typename V1, typename V2>
struct conditional_template_type_constant_c;

template<typename V1, typename V2>
struct conditional_template_type_constant_c<true, V1, V2> {
    template<typename T>
    using type = typename V1::template value<T>;
};

template<typename V1, typename V2>
struct conditional_template_type_constant_c<false, V1, V2> {
    template<typename T>
    using type = typename V2::template value<T>;
};

} //end of namespace cpp

#endif //CPP_UTILS_TMP_HPP

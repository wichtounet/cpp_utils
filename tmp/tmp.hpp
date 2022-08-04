//=======================================================================
// Copyright (c) 2013-2020 Baptiste Wicht.
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

/*!
 * \brief TMP helper implementing the NOT boolean operator taking a boolean constant value.
 *
 * Represents an integral constant with the negated value of the given boolean constant.
 *
 * \tparam B The boolean value to negate
 */
template <bool B>
using not_u = std::bool_constant<!B>;

template <bool B>
inline constexpr bool not_u_v = !B;

/*!
 * \brief TMP helper implementing the NOT boolean operator taking a TMP class.
 *
 * Represents an integral constant with the negated value of the given TMP class.
 *
 * \tparam C The TMP class whose value to negate.
 */
template <typename C>
using not_c = std::bool_constant<!C::value>;

template <typename C>
inline constexpr bool not_c_v = !C::value;

/*!
 * \brief TMP helper implementing the AND boolean operator of given boolean values.
 *
 * Represents an integral constant with the ANDed value of the given boolean values.
 *
 * \tparam C The sequence of boolean values to AND.
 */
template <bool... C>
using and_u = std::bool_constant<(... && C)>;

template <bool... C>
inline constexpr bool and_u_v = (... && C);

/*!
 * \brief TMP helper implementing the AND boolean operator taking a sequence TMP class.
 *
 * Represents an integral constant with the ANDed value of the given TMP classes.
 *
 * \tparam C The sequence of TMP classes whose value to AND.
 */
template <typename... C>
using and_c = std::bool_constant<(... && C::value)>;

template <typename... C>
inline constexpr bool and_c_v = (... && C::value);

/*!
 * \brief TMP helper implementing the OR boolean operator of given boolean values.
 *
 * Represents an integral constant with the ANDed value of the given boolean values.
 *
 * \tparam C The sequence of boolean values to OR.
 */
template <bool... C>
using or_u = std::bool_constant<(... || C)>;

template <bool... C>
inline constexpr bool or_u_v = (... || C);

/*!
 * \brief TMP helper implementing the OR boolean operator taking a sequence TMP class.
 *
 * Represents an integral constant with the ANDed value of the given TMP classes.
 *
 * \tparam C The sequence of TMP classes whose value to OR.
 */
template <typename... C>
using or_c = std::bool_constant<(... || C::value)>;

template <typename... C>
inline constexpr bool or_c_v = (... || C::value);

/*!
 * \brief Traits to add const and lvalue reference to T.
 * \tparam T the type to add const and lvalue reference to.
 */
template <typename T>
using add_const_lvalue_t = std::add_lvalue_reference_t<std::add_const_t<T>>;

/*!
 * \brief Traits to add const and rvalue reference to T.
 * \tparam T the type to add const and rvalue reference to.
 */
template <typename T>
using add_const_rvalue_t = std::add_rvalue_reference_t<std::add_const_t<T>>;

/*!
 * \brief Base class for an integral boolean constant extracting its value from a TMP class.
 * \tparam C The TMP class to extract the value from.
 */
template <typename T, typename C>
using integral_constant_c = std::integral_constant<T, C::value>;

/*!
 * \brief Base class for an integral boolean constant.
 * \tparam C The TMP class to extract the boolean value from.
 */
template <typename C>
using bool_constant_c = std::bool_constant<C::value>;

/*!
 * \brief Base class for an integral constant of auto type.
 *
 * The type of the integral constant is automatically deduced from the the value member of the given TMP class.
 *
 * \tparam V The TMP class to extract the value from.
 */
template <typename V>
struct auto_constant : std::integral_constant<decltype(V::value), V::value> {};

template <typename V>
inline constexpr decltype(auto) auto_constant_v = V::value;


/*!
 * \brief A conditional constant extracting its value from either V1 or V2 depending on the condition
 * \tparam C The boolean value
 * \tparam V1 The first value class
 * \tparam V2 The second value class
 */
template <bool C, typename V1, typename V2>
struct conditional_constant;

/*!
 * \copydoc conditional_constant
 */
template <typename V1, typename V2>
struct conditional_constant<true, V1, V2> : auto_constant<V1> {};

/*!
 * \copydoc conditional_constant
 */
template <typename V1, typename V2>
struct conditional_constant<false, V1, V2> : auto_constant<V2> {};

template <bool C, typename V1, typename V2>
inline constexpr decltype(auto) conditional_constant_v = conditional_constant<C, V1, V2>::value;

/*!
 * \brief Base class to define a type constant
 * \tparam T The type constant.
 */
template <typename T>
struct type_constant {
    using type = T; ///< The result type
};

/*!
 * \brief Base class to define a type constant
 * \tparam T The type to extract the type constant from.
 */
template <typename T>
struct type_constant_c {
    using type = typename T::value; ///< The result type
};

/*!
 * \brief A conditional type constant whose type is either V1 or V2 depending on the condition
 * \tparam C The boolean value
 * \tparam V1 The first type
 * \tparam V2 The second type
 */
template <bool C, typename V1, typename V2>
struct conditional_type_constant;

/*!
 * \copydoc conditional_type_constant
 */
template <typename V1, typename V2>
struct conditional_type_constant<true, V1, V2> {
    using type = V1; ///< The result type
};

/*!
 * \copydoc conditional_type_constant
 */
template <typename V1, typename V2>
struct conditional_type_constant<false, V1, V2> {
    using type = V2; ///< The result type
};

/*!
 * \brief A conditional type constant extracting its type from either V1 or V2 depending on the condition
 * \tparam C The boolean value
 * \tparam V1 The first type value class
 * \tparam V2 The second type value class
 */
template <bool C, typename V1, typename V2>
struct conditional_type_constant_c;

/*!
 * \copydoc conditional_type_constant_c
 */
template <typename V1, typename V2>
struct conditional_type_constant_c<true, V1, V2> {
    using type = typename V1::value; ///< The result type
};

/*!
 * \copydoc conditional_type_constant_c
 */
template <typename V1, typename V2>
struct conditional_type_constant_c<false, V1, V2> {
    using type = typename V2::value; ///< The result type
};

/*!
 * \brief Base class to define a template type constant
 * \tparam TT The template type constant.
 */
template <typename TT>
struct template_type_constant {
    /*!
     * \brief The result type
     */
    template <typename T>
    using type = TT;
};

/*!
 * \brief Base class to define a template type constant
 * \tparam TT The type to extract the template type constant from.
 */
template <typename TT>
struct template_type_constant_c {
    /*!
     * \brief The result type
     */
    template <typename T>
    using type = typename TT::template value<T>;
};

/*!
 * \brief A conditional template type constant whose type is either V1 or V2 depending on the condition
 * \tparam C The boolean value
 * \tparam V1 The first template type
 * \tparam V2 The second template type
 */
template <bool C, template <typename> class V1, template <typename> class V2>
struct conditional_template_type_constant;

/*!
 * \brief conditional_template_type_constant
 */
template <template <typename> class V1, template <typename> class V2>
struct conditional_template_type_constant<true, V1, V2> {
    /*!
     * \brief The result type
     */
    template <typename T>
    using type = V1<T>;
};

/*!
 * \brief conditional_template_type_constant
 */
template <template <typename> class V1, template <typename> class V2>
struct conditional_template_type_constant<false, V1, V2> {
    /*!
     * \brief The result type
     */
    template <typename T>
    using type = V2<T>;
};

/*!
 * \brief A conditional template type constant extracting its template type from either V1 or V2 depending on the condition
 * \tparam C The boolean value
 * \tparam V1 The first template type value class
 * \tparam V2 The second template type value class
 */
template <bool C, typename V1, typename V2>
struct conditional_template_type_constant_c;

/*!
 * \brief conditional_template_type_constant_c
 */
template <typename V1, typename V2>
struct conditional_template_type_constant_c<true, V1, V2> {
    /*!
     * \brief The result type
     */
    template <typename T>
    using type = typename V1::template value<T>;
};

/*!
 * \brief conditional_template_type_constant_c
 */
template <typename V1, typename V2>
struct conditional_template_type_constant_c<false, V1, V2> {
    /*!
     * \brief The result type
     */
    template <typename T>
    using type = typename V2::template value<T>;
};

} //end of namespace cpp

#define HAS_MEM_FUNC(func, name)                                              \
template<typename T, typename Sign>                                           \
struct name {                                                                 \
    typedef char yes[1];                                                      \
    typedef char no [2];                                                      \
    template <typename U, U> struct type_check;                               \
    template <typename _1> static yes &chk(type_check<Sign, &_1::func> *);    \
    template <typename   > static no  &chk(...);                              \
    static constexpr const bool value = sizeof(chk<T>(0)) == sizeof(yes);     \
}

#define HAS_STATIC_FIELD(field, name)                                                       \
template <class T>                                                                          \
class name {                                                                                \
    template<typename U, typename =                                                         \
        typename std::enable_if<!std::is_member_pointer<decltype(&U::field)>::value>::type> \
    static std::true_type check(int);                                                       \
    template <typename>                                                                     \
    static std::false_type check(...);                                                      \
public:                                                                                     \
    static constexpr const bool value = decltype(check<T>(0))::value;                       \
}

#endif //CPP_UTILS_TMP_HPP

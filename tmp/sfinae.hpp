//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains Template Metaprogramming (TMP) utilities for SFINAE.
 *
 * Clang compiler tries to do smart error reporting based on enable-if. For
 * this reason, all the aliases are defined in terms of enable-if and not by
 * composition. Nevertheless, even with that, error reporting fails short when
 * enable_if is not used directly. Therefore, it is advised to use the macros
 * instead of the aliases.
 */

#ifndef CPP_UTILS_TMP_SFINAE_HPP
#define CPP_UTILS_TMP_SFINAE_HPP

namespace cpp {

/*!
 * \brief Helper to use a enable-if-not (disable-if)
 *
 * This can be used to improve readibility of the code
 *
 * \tparam B The boolean disabler
 * \tparam T The type in case of success
 */
template <bool B, typename T = void>
using disable_if             = std::enable_if<!B, T>;

/*!
 * \brief Helper to use a enable-if-not (disable-if), this helper is directly
 * the type of the disabler.
 *
 * This can be used to improve readibility of the code
 *
 * \tparam B The boolean disabler
 * \tparam T The type in case of success
 */
template <bool B, typename T = void>
using disable_if_t           = typename std::enable_if<!B, T>::type;

namespace detail {

//Note: Unfortunately, CLang is bugged (Bug 11723), therefore, it is not
//possible to use universal enable_if/disable_if directly, it is necessary to
//use the dummy :( FU Clang!

/*!
 * \brief Simple enum type for enable_if to return
 */
enum class enabler_t {
    DUMMY ///< The dummy value to assign to enabler-if template type default value
};

/*!
 * \brief Simple dummy constant to use as value in SFINAE
 */
constexpr const enabler_t dummy = enabler_t::DUMMY;

} //end of detail

//Note: For error reporting reasons, it is not a good idea to define these next
//utilities using each other (for instance enable_if_c using enable_if_u)
//CLang tries to report errors of disabling for SFINAE but does a very poor job
//handling enable_if_t, therefore std::enable_if is used directly
//Even with this, the error reporting using these alias declarations is pretty
//bad, this is the reason, the macros have been written :(

/*!
 * \brief Universal enable_if helper, the success-type is a dummy type.
 * \tparam B The boolean enabler
 */
template <bool B>
using enable_if_u = typename std::enable_if<B, detail::enabler_t>::type;

/*!
 * \brief Universal disable_if helper, the success-type is a dummy type.
 * \tparam B The boolean disabler
 */
template <bool B>
using disable_if_u = typename std::enable_if<not_u<B>::value, detail::enabler_t>::type;

/*!
 * \brief Universal enable_if helper from traits, the success-type is a dummy type.
 * \tparam C The traits to extract the value from.
 */
template <typename C>
using enable_if_c = typename std::enable_if<C::value, detail::enabler_t>::type;

/*!
 * \brief Universal disable_if helper from traits, the success-type is a dummy type.
 * \tparam C The traits to extract the value from.
 */
template <typename C>
using disable_if_c = typename std::enable_if<not_c<C>::value, detail::enabler_t>::type;

/*!
 * \brief Universal enable_if helper for several boolean values (AND), the success-type is a dummy type.
 * \tparam B The boolean values for enabler
 */
template <bool... B>
using enable_if_all_u = typename std::enable_if<and_u<B...>::value, detail::enabler_t>::type;

/*!
 * \brief Universal disable_if helper for several boolean values (AND), the success-type is a dummy type.
 * \tparam B The boolean values for disabler
 */
template <bool... B>
using disable_if_all_u = typename std::enable_if<not_c<and_u<B...>>::value, detail::enabler_t>::type;

/*!
 * \brief Universal enable_if helper for several traits (AND), the success-type is a dummy type.
 * \tparam C The traits for enabler
 */
template <typename... C>
using enable_if_all_c = typename std::enable_if<and_c<C...>::value, detail::enabler_t>::type;

/*!
 * \brief Universal disable_if helper for several traits (AND), the success-type is a dummy type.
 * \tparam C The traits for disabler
 */
template <typename... C>
using disable_if_all_c = typename std::enable_if<not_c<and_c<C...>>::value, detail::enabler_t>::type;

/*!
 * \brief Universal enable_if helper for several boolean values (OR), the success-type is a dummy type.
 * \tparam B The boolean values for enabler
 */
template <bool... B>
using enable_if_one_u = typename std::enable_if<or_u<B...>::value, detail::enabler_t>::type;

/*!
 * \brief Universal disable_if helper for several boolean values (OR), the success-type is a dummy type.
 * \tparam B The boolean values for disabler
 */
template <bool... B>
using disable_if_one_u = typename std::enable_if<not_c<or_u<B...>>::value, detail::enabler_t>::type;

/*!
 * \brief Universal enable_if helper for several traits (OR), the success-type is a dummy type.
 * \tparam C The traits for enabler
 */
template <typename... C>
using enable_if_one_c = typename std::enable_if<or_c<C...>::value, detail::enabler_t>::type;

/*!
 * \brief Universal disable_if helper for several traits (OR), the success-type is a dummy type.
 * \tparam C The traits for disabler
 */
template <typename... C>
using disable_if_one_c = typename std::enable_if<not_c<or_c<C...>>::value, detail::enabler_t>::type;

//For the same reasons, the macros are defined using std::enable_if and not the
//simpler versions
//Note: We don't surround args by parenths, since this would result in being evaluated as the comma operator

#ifndef SONAR_ANALYSIS

#define cpp_enable_if(...) typename std::enable_if<cpp::and_u<__VA_ARGS__>::value, cpp::detail::enabler_t>::type = cpp::detail::dummy
#define cpp_disable_if(...) typename std::enable_if<cpp::not_c<cpp::and_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>::type = cpp::detail::dummy

#define cpp_enable_if_fwd(...) typename std::enable_if<cpp::and_u<__VA_ARGS__>::value, cpp::detail::enabler_t>::type
#define cpp_disable_if_fwd(...) typename std::enable_if<cpp::not_c<cpp::and_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>::type

#define cpp_enable_if_or(...) typename std::enable_if<cpp::or_u<__VA_ARGS__>::value, cpp::detail::enabler_t>::type = cpp::detail::dummy
#define cpp_disable_if_or(...) typename std::enable_if<cpp::not_c<cpp::or_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>::type = cpp::detail::dummy

#define cpp_enable_if_or_fwd(...) typename std::enable_if<cpp::or_u<__VA_ARGS__>::value, cpp::detail::enabler_t>::type
#define cpp_disable_if_or_fwd(...) typename std::enable_if<cpp::not_c<cpp::or_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>::type

#define cpp_enable_if_cst(...) bool CPP_CST_ENABLE = true, typename std::enable_if < (__VA_ARGS__) && CPP_CST_ENABLE, cpp::detail::enabler_t > ::type = cpp::detail::dummy
#define cpp_disable_if_cst(...) bool CPP_CST_ENABLE = true, typename std::enable_if<!((__VA_ARGS__) && CPP_CST_ENABLE), cpp::detail::enabler_t>::type = cpp::detail::dummy

#else

//sonar-cxx does a very poor job of handling the macros directly. Therefore we simply use std::enable_if_t to avoid false positives by the hundreds
//These versions cannot be used in the general case for debugging reasons (see explanations above)

#define cpp_enable_if(...) std::enable_if_t<cpp::and_u<__VA_ARGS__>::value, cpp::detail::enabler_t> = cpp::detail::dummy
#define cpp_disable_if(...) std::enable_if_t<cpp::not_c<cpp::and_u<__VA_ARGS__>>::value, cpp::detail::enabler_t> = cpp::detail::dummy

#define cpp_enable_if_fwd(...) std::enable_if_t<cpp::and_u<__VA_ARGS__>::value, cpp::detail::enabler_t>
#define cpp_disable_if_fwd(...) std::enable_if_t<cpp::not_c<cpp::and_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>

#define cpp_enable_if_or(...) std::enable_if_t<cpp::or_u<__VA_ARGS__>::value, cpp::detail::enabler_t> = cpp::detail::dummy
#define cpp_disable_if_or(...) std::enable_if_t<cpp::not_c<cpp::or_u<__VA_ARGS__>>::value, cpp::detail::enabler_t> = cpp::detail::dummy

#define cpp_enable_if_or_fwd(...) std::enable_if_t<cpp::or_u<__VA_ARGS__>::value, cpp::detail::enabler_t>
#define cpp_disable_if_or_fwd(...) std::enable_if_t<cpp::not_c<cpp::or_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>

#define cpp_enable_if_cst(...) bool CPP_CST_ENABLE = true, std::enable_if_t < (__VA_ARGS__) && CPP_CST_ENABLE, cpp::detail::enabler_t > = cpp::detail::dummy
#define cpp_disable_if_cst(...) bool CPP_CST_ENABLE = true, std::enable_if_t<!((__VA_ARGS__) && CPP_CST_ENABLE), cpp::detail::enabler_t> = cpp::detail::dummy

#endif

} //end of namespace cpp

#endif //CPP_UTILS_TMP_HPP

//=======================================================================
// Copyright (c) 2013-2014-2015 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains Template Metaprogramming (TMP) utilities for SFINAE.
 */

#ifndef CPP_UTILS_TMP_SFINAE_HPP
#define CPP_UTILS_TMP_SFINAE_HPP

namespace cpp {

//enable_if utilities

template<bool B, typename T = void>
using disable_if = std::enable_if<!B, T>;

template<bool B, typename T = void>
using disable_if_t = typename std::enable_if<!B, T>::type;

namespace detail {

//Note: Unfortunately, CLang is bugged (Bug 11723), therefore, it is not
//possible to use universal enable_if/disable_if directly, it is necessary to
//use the dummy :( FU Clang!

enum class enabler_t { DUMMY };
constexpr const enabler_t dummy = enabler_t::DUMMY;

} //end of detail

//Note: For error reporting reasons, it is not a good idea to define these next
//utilities using each other (for instance enable_if_c using enable_if_u)
//CLang tries to report errors of disabling for SFINAE but does a very poor job
//handling enable_if_t, therefore std::enable_if is used directly
//Even with this, the error reporting using these alias declarations is pretty
//bad, this is the reason, the macros have been written :(

template<bool B>
using enable_if_u = typename std::enable_if<B, detail::enabler_t>::type;

template<bool B>
using disable_if_u = typename std::enable_if<not_u<B>::value, detail::enabler_t>::type;

template<typename C>
using enable_if_c = typename std::enable_if<C::value, detail::enabler_t>::type;

template<typename C>
using disable_if_c = typename std::enable_if<not_c<C>::value, detail::enabler_t>::type;

template<bool... B>
using enable_if_all_u = typename std::enable_if<and_u<B...>::value, detail::enabler_t>::type;

template<bool... B>
using disable_if_all_u = typename std::enable_if<not_c<and_u<B...>>::value, detail::enabler_t>::type;

template<typename... C>
using enable_if_all_c = typename std::enable_if<and_c<C...>::value, detail::enabler_t>::type;

template<typename... C>
using disable_if_all_c = typename std::enable_if<not_c<and_c<C...>>::value, detail::enabler_t>::type;

template<bool... B>
using enable_if_one_u = typename std::enable_if<or_u<B...>::value, detail::enabler_t>::type;

template<bool... B>
using disable_if_one_u = typename std::enable_if<not_c<or_u<B...>>::value, detail::enabler_t>::type;

template<typename... C>
using enable_if_one_c = typename std::enable_if<or_c<C...>::value, detail::enabler_t>::type;

template<typename... C>
using disable_if_one_c = typename std::enable_if<not_c<or_c<C...>>::value, detail::enabler_t>::type;

//For the same reasons, the macros are defined using std::enable_if and not the
//simpler versions
//Note: We don't surround args by parenths, since this would result in being evaluated as the comma operator

#define cpp_enable_if(...) typename std::enable_if<cpp::and_u<__VA_ARGS__>::value, cpp::detail::enabler_t>::type = cpp::detail::dummy
#define cpp_disable_if(...) typename std::enable_if<cpp::not_c<cpp::and_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>::type= cpp::detail::dummy

#define cpp_enable_if_fwd(...) typename std::enable_if<cpp::and_u<__VA_ARGS__>::value, cpp::detail::enabler_t>::type
#define cpp_disable_if_fwd(...) typename std::enable_if<cpp::not_c<cpp::and_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>::type

#define cpp_enable_if_or(...) typename std::enable_if<cpp::or_u<__VA_ARGS__>::value, cpp::detail::enabler_t>::type = cpp::detail::dummy
#define cpp_disable_if_or(...) typename std::enable_if<cpp::not_c<cpp::or_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>::type= cpp::detail::dummy

#define cpp_enable_if_or_fwd(...) typename std::enable_if<cpp::or_u<__VA_ARGS__>::value, cpp::detail::enabler_t>::type
#define cpp_disable_if_or_fwd(...) typename std::enable_if<cpp::not_c<cpp::or_u<__VA_ARGS__>>::value, cpp::detail::enabler_t>::type

#define cpp_enable_if_cst(...) bool CPP_CST_ENABLE = true, typename std::enable_if<(__VA_ARGS__) && CPP_CST_ENABLE, cpp::detail::enabler_t>::type = cpp::detail::dummy
#define cpp_disable_if_cst(...) bool CPP_CST_ENABLE = true, typename std::enable_if<!((__VA_ARGS__) && CPP_CST_ENABLE), cpp::detail::enabler_t>::type = cpp::detail::dummy

} //end of namespace cpp

#endif //CPP_UTILS_TMP_HPP

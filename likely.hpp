//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file likely.hpp
 * \brief Contains preprocessor macros to help the compiler with branch prediction.
 */

#ifndef CPP_UTILS_LIKELY_HPP
#define CPP_UTILS_LIKELY_HPP

#if defined __clang__

#if __has_builtin(__bultin_expect)
#define cpp_likely(x) __builtin_expect(!!(x), 1)
#define cpp_unlikely(x) __builtin_expect(!!(x), 0)
#else
#define cpp_likely(x) x
#define cpp_unlikely(x) x
#endif

#elif defined __GNUC__

#define cpp_likely(x) __builtin_expect(!!(x), 1)
#define cpp_unlikely(x) __builtin_expect(!!(x), 0)

#endif //__clang__

#endif //CPP_UTILS_LIKELY_HPP

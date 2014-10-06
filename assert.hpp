//=======================================================================
// Copyright (c) 2013-2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_ASSERT_HPP
#define CPP_UTILS_ASSERT_HPP

#include <iostream>

#ifdef CPP_UTILS_ASSERT_EXCEPTION
#include <stdexcept>
#endif

#include "likely.hpp"

#define cpp_unused(x) ((void)x)

#ifdef NDEBUG

#define cpp_assert(condition, message) ((void)0)

#if defined __clang__

#if __has_builtin(__builtin_unreachable)
#define cpp_unreachable(message) __builtin_unreachable();
#else
#define cpp_unreachable(message) ((void)0)
#endif //__has_builtin(__builtin_unreachable)

#elif defined __GNUC__

#define cpp_unreachable(message) __builtin_unreachable();

#endif //__clang__

#else

#ifdef CPP_UTILS_ASSERT_EXCEPTION

#define cpp_assert(condition, message) if(cpp_likely(condition)) \
    ((void)0); \
    else throw std::runtime_error("Assertion failed");

#else

#define cpp_assert(condition, message) (cpp_likely(condition) \
    ? ((void)0) \
    : ::cpp::assertion::detail::assertion_failed_msg(#condition, message, \
    __PRETTY_FUNCTION__, __FILE__, __LINE__))

#endif

#if defined __clang__

#if __has_builtin(__builtin_unreachable)
#define cpp_unreachable(message) cpp_assert(false, message); __builtin_unreachable();
#else
#define cpp_unreachable(message) cpp_assert(false, message);
#endif //__has_builtin(__builtin_unreachable)

#elif defined __GNUC__

#define cpp_unreachable(message) cpp_assert(false, message); __builtin_unreachable();

#endif //__clang__

#endif //NDEBUG

namespace cpp {
namespace assertion {
namespace detail {

template< typename CharT >
void assertion_failed_msg(const CharT* expr, const char* msg, const char* function, const char* file, long line){
    std::cerr
        << "***** Internal Program Error - assertion (" << expr << ") failed in "
        << function << ":\n"
        << file << '(' << line << "): " << msg << std::endl;
    std::abort();
}

} // end of detail namespace
} // end of assertion namespace
} // end of cpp namespace

#endif //CPP_UTILS_ASSERT_HPP

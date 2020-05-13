//=======================================================================
// Copyright (c) 2013-2020 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file string.hpp
 * \brief Contains string manipulation functions.
 */

#ifndef CPP_UTILS_STRING_HPP
#define CPP_UTILS_STRING_HPP

#include <locale>
#include <algorithm>

namespace cpp {

/*!
 * \brief Left trim the given string. 
 * \param s The string to modify
 * \return a reference to the string (s)
 *
 * All the spaces on the left of the string will be removed.
 */
template <typename CharT, typename Traits, typename Allocator>
std::basic_string<CharT, Traits, Allocator>& ltrim(std::basic_string<CharT, Traits, Allocator>& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

/*!
 * \brief Right trim the given string. 
 * \param s The string to modify
 * \return a reference to the string (s)
 *
 * All the spaces on the right of the string will be removed.
 */
template <typename CharT, typename Traits, typename Allocator>
std::basic_string<CharT, Traits, Allocator>& rtrim(std::basic_string<CharT, Traits, Allocator>& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

/*!
 * \brief Trim the given string. 
 * \param s The string to modify
 * \return a reference to the string (s)
 *
 * All the spaces on the left and the right of the string will be removed.
 */
template <typename CharT, typename Traits, typename Allocator>
std::basic_string<CharT, Traits, Allocator>& trim(std::basic_string<CharT, Traits, Allocator>& s) {
    return ltrim(rtrim(s));
}

} //end of the cpp namespace

#endif //CPP_UTILS_STRING_HPP

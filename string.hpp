//=======================================================================
// Copyright (c) 2013-2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_STRING_HPP
#define CPP_UTILS_STRING_HPP

#include <locale>
#include <algorithm>

namespace cpp {

template<typename CharT, typename Traits, typename Allocator> 
std::basic_string<CharT, Traits, Allocator>& ltrim(std::basic_string<CharT, Traits, Allocator>& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

template<typename CharT, typename Traits, typename Allocator> 
std::basic_string<CharT, Traits, Allocator>& rtrim(std::basic_string<CharT, Traits, Allocator>& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

template<typename CharT, typename Traits, typename Allocator> 
std::basic_string<CharT, Traits, Allocator>& trim(std::basic_string<CharT, Traits, Allocator>& s) {
    return ltrim(rtrim(s));
}

} //end of the cpp namespace

#endif //CPP_UTILS_STRING_HPP

//=======================================================================
// Copyright (c) 2013-2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_STREAMS_HPP
#define CPP_UTILS_STREAMS_HPP

#include <iostream>
#include <vector>

namespace cpp {

template<typename T, typename A>
std::ostream& operator<<(std::ostream& os, const std::vector<T, A>& vec) {
    os << "[";
    std::string comma;
    for(auto& v : vec){
        os << comma << v;
        comma = ", ";
    }
    os << "]";
    return os;
}

} //end of the cpp namespace

#endif //CPP_UTILS_STREAMS_HPP

//=======================================================================
// Copyright (c) 2013-2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_PARALLEL_HPP
#define CPP_UTILS_PARALLEL_HPP

#include<thread>

namespace cpp {

template<typename Iterator, typename Functor>
void parallel_foreach(Iterator first, Iterator last, Functor&& fun){
    for(; first != last; ++first){
        std::thread(fun, *first).detach();
    }
}

} //end of the cpp namespace

#endif //CPP_UTILS_PARALLEL_HPP

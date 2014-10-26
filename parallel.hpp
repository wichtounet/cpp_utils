//=======================================================================
// Copyright (c) 2013-2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_PARALLEL_HPP
#define CPP_UTILS_PARALLEL_HPP

#include<thread>
#include<future>

namespace cpp {

template<typename Iterator, typename Functor>
void parallel_foreach(Iterator first, Iterator last, Functor&& fun){
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for(; first != last; ++first){
        futures.push_back(std::move(std::async(std::launch::async, fun, *first)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Container, typename Functor>
void parallel_foreach_i(const Container& container, Functor&& fun){
    std::vector<std::future<void>> futures;
    futures.reserve(container.size());

    for(std::size_t i = 0; i < container.size(); ++i){
        futures.push_back(std::move(std::async(std::launch::async, fun, i)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

} //end of the cpp namespace

#endif //CPP_UTILS_PARALLEL_HPP

//=======================================================================
// Copyright (c) 2013-2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_ALGORITHM_HPP
#define CPP_UTILS_ALGORITHM_HPP

namespace cpp {

template<typename Iterator, typename Functor>
void pairwise_foreach(Iterator first, Iterator last, Functor&& fun){
    for(; first != last; ++first){
        for(auto next = std::next(first); next != last; ++next){
            fun(*first, *next);
        }
    }
}

template<typename Iterator, typename Functor>
void pairwise_foreach_it(Iterator first, Iterator last, Functor&& fun){
    for(; first != last; ++first){
        for(auto next = std::next(first); next != last; ++next){
            fun(first, next);
        }
    }
}

template<typename Iterator, typename Functor>
auto vector_transform(Iterator first, Iterator last, Functor&& fun){
    std::vector<decltype(fun(*first))> transformed;
    std::transform(first, last, std::back_inserter(transformed), std::forward<Functor>(fun));
    return transformed;
}

} //end of the cpp namespace

#endif //CPP_UTILS_ALGORITHM_HPP

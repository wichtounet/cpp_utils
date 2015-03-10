//=======================================================================
// Copyright (c) 2013-2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_ALGORITHM_HPP
#define CPP_UTILS_ALGORITHM_HPP

#include "assert.hpp"

namespace cpp {

//Foreach loops giving the elements to the functor

template<typename Container, typename Functor>
void foreach(Container& container, Functor&& fun) noexcept(noexcept(fun(*container.begin()))) {
    using std::begin;
    using std::end;
    cpp::foreach(begin(end), end(container), std::forward<Functor>(fun));
}

template<typename Iterator, typename Functor>
void foreach(Iterator first, Iterator last, Functor&& fun) noexcept(noexcept(fun(*first))) {
    std::for_each(first, last, std::forward<Functor>(fun));
}

//Foreach loops giving the iterator to the functor

template<typename Container, typename Functor>
void foreach_it(Container& container, Functor&& fun) noexcept(noexcept(fun(container.begin()))) {
    using std::begin;
    using std::end;
    cpp::foreach_it(begin(container), end(container), std::forward<Functor>(fun));
}

template<typename Iterator, typename Functor>
void foreach_it(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(first))) {
    for(; first != last; ++first){
        fun(first);
    }
}

//Foreach loops giving the element and its position to the functor

template<typename Container, typename Functor>
void foreach_i(Container& container, Functor&& fun) noexcept(noexcept(fun(*container.begin(),0))) {
    using std::begin;
    using std::end;
    cpp::foreach_i(begin(container), end(container), std::forward<Functor>(fun));
}

template<typename Iterator, typename Functor>
void foreach_i(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(*first,0))) {
    std::size_t i = 0;
    for(; first != last; ++first, ++i){
        fun(*first, i);
    }
}

//Foreach loop giving all pair of the sequence as elements

template<typename Container, typename Functor>
void foreach_pair(Container& container, Functor&& fun) noexcept(noexcept(fun(*container.begin(), *std::next(container.begin())))) {
    using std::begin;
    using std::end;
    cpp::foreach_pair(begin(container), end(container), std::forward<Functor>(fun));
}

template<typename Iterator, typename Functor>
void foreach_pair(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(*first, *std::next(first)))) {
    for(; first != last; ++first){
        for(auto next = std::next(first); next != last; ++next){
            fun(*first, *next);
        }
    }
}

//Foreach loop giving all pair of the sequence as iterators

template<typename Container, typename Functor>
void foreach_pair_it(Container& container, Functor&& fun) noexcept(noexcept(fun(container.begin(), std::next(container.begin())))) {
    using std::begin;
    using std::end;
    cpp::foreach_pair_it(begin(container), end(container), std::forward<Functor>(fun));
}

template<typename Iterator, typename Functor>
void foreach_pair_it(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(first, std::next(first)))) {
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

template<typename IT1, typename IT2, typename RNG>
void parallel_shuffle(IT1 first_1, IT1 last_1, IT2 first_2, IT2 last_2, RNG&& g){
    cpp_assert(std::distance(first_1, last_1) == std::distance(first_2, last_2), "The two sequences should be of the same size");
    cpp_unused(last_2); //Ensure no warning is issued for last_2 (used only in debug mode)

    using diff_t = typename std::iterator_traits<IT1>::difference_type;
    using udiff_t = typename std::make_unsigned<diff_t>::type;
    using distr_t = typename std::uniform_int_distribution<udiff_t>;
    using param_t = typename distr_t::param_type;

    distr_t D;
    diff_t n = last_1 - first_1;
    for (diff_t i = n-1; i > 0; --i) {
        auto new_i = D(g, param_t(0, i));

        std::swap(first_1[i], first_1[new_i]);
        std::swap(first_2[i], first_2[new_i]);
    }
}

} //end of the cpp namespace

#endif //CPP_UTILS_ALGORITHM_HPP

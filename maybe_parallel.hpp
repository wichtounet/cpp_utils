//=======================================================================
// Copyright (c) 2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file maybe_parallel.hpp
 * \brief Contains algorithms that can either be executed
 * concurrently or linearly based on a compile-time switch.
 */

#ifndef CPP_UTILS_MAYBE_PARALLEL_HPP
#define CPP_UTILS_MAYBE_PARALLEL_HPP

#include "algorithm.hpp"
#include "parallel.hpp"

namespace cpp {

template<bool Parallel>
struct thread_pool {
    //Does not do anything by default
};

template<>
struct thread_pool<true> : default_thread_pool<> {
    //Simply inherits from default thread pool
};

//parallel versions

template<typename Container, typename Functor>
void maybe_parallel_foreach(thread_pool<true>& thread_pool, const Container& container, Functor&& fun){
    parallel_foreach(thread_pool, container, std::forward<Functor>(fun));
}

template<typename Iterator, typename Functor>
void maybe_parallel_foreach(thread_pool<true>& thread_pool, Iterator first, Iterator last, Functor&& fun){
    parallel_foreach(thread_pool, first, last, std::forward<Functor>(fun));
}

template<typename Container, typename Functor>
void maybe_parallel_foreach_i(thread_pool<true>& thread_pool, const Container& container, Functor&& fun){
    parallel_foreach_i(thread_pool, container, std::forward<Functor>(fun));
}

template<typename Iterator, typename Functor>
void maybe_parallel_foreach_i(thread_pool<true>& thread_pool, Iterator it, Iterator end, Functor&& fun){
    parallel_foreach_i(thread_pool, it, end, std::forward<Functor>(fun));
}

template<typename Iterator, typename Iterator2, typename Functor>
void maybe_parallel_foreach_pair_i(thread_pool<true>& thread_pool, Iterator it, Iterator end, Iterator2 iit, Iterator2 ilast, Functor&& fun){
    parallel_foreach_pair_i(thread_pool, it, end, iit, ilast, std::forward<Functor>(fun));
}

template<typename Functor>
void maybe_parallel_foreach_n(thread_pool<true>& thread_pool, std::size_t first, std::size_t last, Functor&& fun){
    parallel_foreach_n(thread_pool, first, last, std::forward<Functor>(fun));
}

//non-parallel versions

template<typename Container, typename Functor>
void maybe_parallel_foreach(thread_pool<false>& /*thread_pool*/, const Container& container, Functor&& fun){
    foreach(container, fun);
}

template<typename Iterator, typename Functor>
void maybe_parallel_foreach(thread_pool<false>& /*thread_pool*/, Iterator first, Iterator last, Functor&& fun){
    foreach(first, last, fun);
}

template<typename Container, typename Functor>
void maybe_parallel_foreach_i(thread_pool<false>& /*thread_pool*/, const Container& container, Functor&& fun){
    foreach_i(container, fun);
}

template<typename Iterator, typename Functor>
void maybe_parallel_foreach_i(thread_pool<false>& /*thread_pool*/, Iterator it, Iterator end, Functor&& fun){
    foreach_i(it, end, fun);
}

template<typename Iterator, typename Iterator2, typename Functor>
void maybe_parallel_foreach_pair_i(thread_pool<false>& /*thread_pool*/, Iterator it, Iterator end, Iterator2 iit, Iterator2 ilast, Functor&& fun){
    cpp_unused(ilast);

    for(std::size_t i = 0; it != end; ++it, ++iit, ++i){
        fun(*it, *iit, i);
    }
}

template<typename functor>
void maybe_parallel_foreach_n(thread_pool<false>& /*thread_pool*/, std::size_t first, std::size_t last, functor&& fun){
    foreach_n(first, last, fun);
}

} //end of dll namespace

#endif

//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
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

/*!
 * \brief A conditional thread pool
 * \tparam Parallel Defines if the thread pool is parallel or not
 */
template <bool Parallel>
struct thread_pool {
    /*!
     * \brief Construct the thread pool and forward the args to the
     * real implementation (if any)
     */
    template<typename... Args>
    explicit thread_pool(Args... /*args*/){
        //Does not do anything by default
    }
};

/*!
 * \copydoc thread_pool
 */
template <>
struct thread_pool<true> : default_thread_pool<> {
    /*!
     * \brief Construct the thread pool and forward the args to the
     * real implementation (if any)
     */
    template<typename... Args>
    explicit thread_pool(Args... args) : default_thread_pool<>(std::forward<Args>(args)...){
        //Simply inherits from default thread pool
    }
};

//parallel versions

/*!
 * \brief Apply the given function to each element of the given
 * container, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param container The container to iterate
 * \param fun The functor to apply
 */
template <typename Container, typename Functor>
void maybe_parallel_foreach(thread_pool<true>& thread_pool, const Container& container, Functor&& fun) {
    parallel_foreach(thread_pool, container, std::forward<Functor>(fun));
}

/*!
 * \brief Apply the given function to each element of the given
 * range, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply
 */
template <typename Iterator, typename Functor>
void maybe_parallel_foreach(thread_pool<true>& thread_pool, Iterator first, Iterator last, Functor&& fun) {
    parallel_foreach(thread_pool, first, last, std::forward<Functor>(fun));
}

/*!
 * \brief Apply the given function to each element of the given
 * container and its position, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param container The container to iterate
 * \param fun The functor to apply
 */
template <typename Container, typename Functor>
void maybe_parallel_foreach_i(thread_pool<true>& thread_pool, const Container& container, Functor&& fun) {
    parallel_foreach_i(thread_pool, container, std::forward<Functor>(fun));
}

/*!
 * \brief Apply the given function to each element of the given
 * range and its position, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param it The beginning of the range
 * \param end The end of the range
 * \param fun The functor to apply
 */
template <typename Iterator, typename Functor>
void maybe_parallel_foreach_i(thread_pool<true>& thread_pool, Iterator it, Iterator end, Functor&& fun) {
    parallel_foreach_i(thread_pool, it, end, std::forward<Functor>(fun));
}

/*!
 * \brief Apply the given function to each pair of element of the given
 * ranges and its position, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param it The beginning of the first range
 * \param end The end of the first range
 * \param iit The beginning of the second range
 * \param ilast The end of the second range
 * \param fun The functor to apply
 */
template <typename Iterator, typename Iterator2, typename Functor>
void maybe_parallel_foreach_pair_i(thread_pool<true>& thread_pool, Iterator it, Iterator end, Iterator2 iit, Iterator2 ilast, Functor&& fun) {
    parallel_foreach_pair_i(thread_pool, it, end, iit, ilast, std::forward<Functor>(fun));
}

/*!
 * \brief Apply the given function to each position in the given
 * range, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply
 */
template <typename Functor>
void maybe_parallel_foreach_n(thread_pool<true>& thread_pool, std::size_t first, std::size_t last, Functor&& fun) {
    parallel_foreach_n(thread_pool, first, last, std::forward<Functor>(fun));
}

//non-parallel versions

/*!
 * \brief Apply the given function to each element of the given
 * container, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param container The container to iterate
 * \param fun The functor to apply
 */
template <typename Container, typename Functor>
void maybe_parallel_foreach(thread_pool<false>& thread_pool, const Container& container, Functor&& fun) {
    cpp_unused(thread_pool);
    foreach (container, fun);
}

/*!
 * \brief Apply the given function to each element of the given
 * range, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply
 */
template <typename Iterator, typename Functor>
void maybe_parallel_foreach(thread_pool<false>& thread_pool, Iterator first, Iterator last, Functor&& fun) {
    cpp_unused(thread_pool);

    foreach (first, last, fun);
}

/*!
 * \brief Apply the given function to each element of the given
 * container and its position, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param container The container to iterate
 * \param fun The functor to apply
 */
template <typename Container, typename Functor>
void maybe_parallel_foreach_i(thread_pool<false>& thread_pool, const Container& container, Functor&& fun) {
    cpp_unused(thread_pool);
    foreach_i(container, fun);
}

/*!
 * \brief Apply the given function to each element of the given
 * range and its position, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param it The beginning of the range
 * \param end The end of the range
 * \param fun The functor to apply
 */
template <typename Iterator, typename Functor>
void maybe_parallel_foreach_i(thread_pool<false>& thread_pool, Iterator it, Iterator end, Functor&& fun) {
    cpp_unused(thread_pool);
    foreach_i(it, end, fun);
}

/*!
 * \brief Apply the given function to each pair of element of the given
 * ranges and its position, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param it The beginning of the first range
 * \param end The end of the first range
 * \param iit The beginning of the second range
 * \param ilast The end of the second range
 * \param fun The functor to apply
 */
template <typename Iterator, typename Iterator2, typename Functor>
void maybe_parallel_foreach_pair_i(thread_pool<false>& thread_pool, Iterator it, Iterator end, Iterator2 iit, Iterator2 ilast, Functor&& fun) {
    cpp_unused(thread_pool);
    cpp_unused(ilast);

    for (std::size_t i = 0; it != end; ++it, ++iit, ++i) {
        fun(*it, *iit, i);
    }
}

/*!
 * \brief Apply the given function to each position in the given
 * range, in parallel if the thread pool is paralle,
 * sequentially otherwise.
 * \param thread_pool The thread pool
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply
 */
template <typename functor>
void maybe_parallel_foreach_n(thread_pool<false>& thread_pool, std::size_t first, std::size_t last, functor&& fun) {
    cpp_unused(thread_pool);

    foreach_n(first, last, fun);
}

} //end of dll namespace

#endif

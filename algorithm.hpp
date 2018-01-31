//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file algorithm.hpp
 * \brief Contains various STL-Like algorithms.
 */

#ifndef CPP_UTILS_ALGORITHM_HPP
#define CPP_UTILS_ALGORITHM_HPP

#include <algorithm> // for vector_transform (std::transform)
#include <random>    // for shuffle algorithms
#include <vector>    // for vector_transform (std::vector)

#include "assert.hpp"

namespace cpp {

/*!
 * \brief Applies the given functor to the result of dereferencing every iterator in the range [first, last) in order.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void foreach (Iterator first, Iterator last, Functor && fun) noexcept(noexcept(fun(*first))) {
    while (first != last) {
        fun(*first++);
    }
}

/*!
 * \brief Applies the given functor to each element of the given container.
 * \param container The container.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void foreach (Container& container, Functor && fun) noexcept(noexcept(fun(*container.begin()))) {
    using std::begin;
    using std::end;
    cpp::foreach (begin(container), end(container), std::forward<Functor>(fun));
}

/*!
 * \brief Applies the given functor to every iterator in the range [first, last) in order.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void foreach_it(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(first))) {
    for (; first != last; ++first) {
        fun(first);
    }
}

/*!
 * \brief Applies the given functor to every iterator in the container in order.
 * \param container The container.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void foreach_it(Container& container, Functor&& fun) noexcept(noexcept(fun(container.begin()))) {
    using std::begin;
    using std::end;
    cpp::foreach_it(begin(container), end(container), std::forward<Functor>(fun));
}

/*!
 * \brief Applies the given functor to the result of dereferencing every iterator in the range [first, last) and its position starting from 0 in order.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void foreach_i(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(*first, 0))) {
    std::size_t i = 0;
    for (; first != last; ++first, ++i) {
        fun(*first, i);
    }
}

/*!
 * \brief Applies the given functor to every value in the container and its position starting from 0 in order.
 * \param container The container.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void foreach_i(Container& container, Functor&& fun) noexcept(noexcept(fun(*container.begin(), 0))) {
    using std::begin;
    using std::end;
    cpp::foreach_i(begin(container), end(container), std::forward<Functor>(fun));
}

/*!
 * \brief Applies the given functor to position the range [first, last) and its position starting from 0 in order.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void foreach_i_only(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(0))) {
    std::size_t i = 0;
    for (; first != last; ++first, ++i) {
        fun(i);
    }
}

/*!
 * \brief Applies the given functor to every position in the given container, in order.
 * \param container The container.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void foreach_i_only(Container& container, Functor&& fun) noexcept(noexcept(fun(0))) {
    using std::begin;
    using std::end;
    cpp::foreach_i_only(begin(container), end(container), std::forward<Functor>(fun));
}

/*!
 * \brief Call the given functor with each contiguous pair of element in the given range, in order.
 * \param first The beginning of the range.
 * \param last The end of the range.
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void foreach_pair(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(*first, *std::next(first)))) {
    for (; first != last; ++first) {
        for (auto next = std::next(first); next != last; ++next) {
            fun(*first, *next);
        }
    }
}

/*!
 * \brief Call the given functor with each contiguous pair of element in the given container, in order.
 * \param container The container to iterate.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void foreach_pair(Container& container, Functor&& fun) noexcept(noexcept(fun(*container.begin(), *std::next(container.begin())))) {
    using std::begin;
    using std::end;
    cpp::foreach_pair(begin(container), end(container), std::forward<Functor>(fun));
}

/*!
 * \brief Call the given functor with the iterators pointing to each contiguous pair of element in the given range, in order.
 * \param first The beginning of the range.
 * \param last The end of the range.
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void foreach_pair_it(Iterator first, Iterator last, Functor fun) noexcept(noexcept(fun(first, std::next(first)))) {
    for (; first != last; ++first) {
        for (auto next = std::next(first); next != last; ++next) {
            fun(first, next);
        }
    }
}

/*!
 * \brief Call the given functor with the iterators pointing to each contiguous pair of element in the given container, in order.
 * \param container The container to iterate.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void foreach_pair_it(Container& container, Functor&& fun) noexcept(noexcept(fun(container.begin(), std::next(container.begin())))) {
    using std::begin;
    using std::end;
    cpp::foreach_pair_it(begin(container), end(container), std::forward<Functor>(fun));
}

/*!
 * \brief Applies the given binary functor to the result of dereferencing every iterator in the ranges [first, last) and [second, ) in order.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param second The beginning of the second range
 * \param fun The functor to apply.
 */
template <typename IT1, typename IT2, typename Functor>
void foreach_dual(IT1 first, IT1 last, IT2 second, Functor fun) noexcept(noexcept(fun(*first, *second))) {
    for (; first != last; ++first, ++second) {
        fun(*first, *second);
    }
}

/*!
 * \brief Applies the given binary functor to each elements of the two given containers in order.
 * \param c1 The first container
 * \param c2 The second container
 * \param fun The functor to apply.
 */
template <typename C1, typename C2, typename Functor>
void foreach_dual(C1& c1, C2& c2, Functor&& fun) noexcept(noexcept(fun(*c1.begin(), *c2.begin()))) {
    using std::begin;
    using std::end;
    cpp::foreach_dual(begin(c1), end(c1), begin(c2), std::forward<Functor>(fun));
}

/*!
 * \brief Applies the given binary functor to the every iterator in the ranges [first, last) and [second, ) in order.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param second The beginning of the second range
 * \param fun The functor to apply.
 */
template <typename IT1, typename IT2, typename Functor>
void foreach_dual_it(IT1 first, IT1 last, IT2 second, Functor fun) noexcept(noexcept(fun(first, second))) {
    for (; first != last; ++first, ++second) {
        fun(first, second);
    }
}

/*!
 * \brief Applies the given binary functor to each iterators of the two given containers in order.
 * \param c1 The first container
 * \param c2 The second container
 * \param fun The functor to apply.
 */
template <typename C1, typename C2, typename Functor>
void foreach_dual_it(C1& c1, C2& c2, Functor&& fun) noexcept(noexcept(fun(c1.begin(), c2.begin()))) {
    using std::begin;
    using std::end;
    cpp::foreach_dual_it(begin(c1), end(c1), begin(c2), std::forward<Functor>(fun));
}

/*!
 * \brief Applies the given binary functor to the result of dereferencing every iterator in the ranges [first, last) and [second, ) and their index in order.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param second The beginning of the second range
 * \param fun The functor to apply.
 */
template <typename IT1, typename IT2, typename Functor>
void foreach_dual_i(IT1 first, IT1 last, IT2 second, Functor fun) noexcept(noexcept(fun(*first, *second, 0))) {
    std::size_t i = 0;
    for (; first != last; ++first, ++second, ++i) {
        fun(*first, *second, i);
    }
}

/*!
 * \brief Applies the given binary functor to each elements of the two given containers and their index in order.
 * \param c1 The first container
 * \param c2 The second container
 * \param fun The functor to apply.
 */
template <typename C1, typename C2, typename Functor>
void foreach_dual_i(C1& c1, C2& c2, Functor&& fun) noexcept(noexcept(fun(*c1.begin(), *c2.begin(), 0))) {
    using std::begin;
    using std::end;
    cpp::foreach_dual_i(begin(c1), end(c1), begin(c2), std::forward<Functor>(fun));
}

/*!
 * \brief Applies the given functor to each value in in range [first, last), in order
 * \param first The first value in the range
 * \param last The last value in the range (not included)
 * \param fun The functor to apply
 */
template <typename Functor>
void foreach_n(std::size_t first, std::size_t last, Functor&& fun) {
    for (std::size_t i = first; i != last; ++i) {
        fun(i);
    }
}

// Create a new vector with the transformation of a sequence

/*!
 * \brief Transform a range and store the result into a vector
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The transform functor
 * \return A vector filled with the transformed objects
 */
template <typename Iterator, typename Functor>
auto vector_transform(Iterator first, Iterator last, Functor&& fun) {
    std::vector<decltype(fun(*first))> transformed;
    std::transform(first, last, std::back_inserter(transformed), std::forward<Functor>(fun));
    return transformed;
}

/*!
 * \brief Shuffles the sequences [first_1, last_1) and [first_2, last_2] so that the same random reordering is chosen for the two sequences.
 * \param first_1 The beginning of the first range.
 * \param last_1 The end of the first range.
 * \param first_2 The beginning of the second range.
 * \param last_2 The end of the second range.
 * \param g A random generator.
 */
template <typename IT1, typename IT2, typename RNG>
void parallel_shuffle(IT1 first_1, IT1 last_1, IT2 first_2, IT2 last_2, RNG&& g) {
    cpp_assert(std::distance(first_1, last_1) == std::distance(first_2, last_2), "The two sequences should be of the same size");
    cpp_unused(last_2); //Ensure no warning is issued for last_2 (used only in debug mode)

    using diff_t  = typename std::iterator_traits<IT1>::difference_type;
    using udiff_t = typename std::make_unsigned<diff_t>::type;
    using distr_t = typename std::uniform_int_distribution<udiff_t>;
    using param_t = typename distr_t::param_type;

    distr_t D;
    diff_t n = last_1 - first_1;
    for (diff_t i = n - 1; i > 0; --i) {
        auto new_i = D(g, param_t(0, i));

        using std::swap;
        swap(first_1[i], first_1[new_i]);
        swap(first_2[i], first_2[new_i]);
    }
}

/*!
 * \brief Shuffles the sequences [first_1, last_1) and [first_2, last_2] so that the same random reordering is chosen for the two sequences.
 * \param first_1 The beginning of the first range.
 * \param last_1 The end of the first range.
 * \param first_2 The beginning of the second range.
 * \param last_2 The end of the second range.
 */
template <typename IT1, typename IT2>
void parallel_shuffle(IT1 first_1, IT1 last_1, IT2 first_2, IT2 last_2) {
    cpp_assert(std::distance(first_1, last_1) == std::distance(first_2, last_2), "The two sequences should be of the same size");
    cpp_unused(last_2); //Ensure no warning is issued for last_2 (used only in debug mode)

    std::random_device rd;
    std::default_random_engine g(rd());

    parallel_shuffle(first_1, last_1, first_2, last_2, g);
}

} //end of the cpp namespace

#endif //CPP_UTILS_ALGORITHM_HPP

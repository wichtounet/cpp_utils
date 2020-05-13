//=======================================================================
// Copyright (c) 2013-2020 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains an std::array-like implementation but with aligned data
 * access.
 *
 * This implementation is limited to C++17. In earlier versions, it will only be
 * aligned for stack-allocation variables, but not when dynamically allocated.
 * In C++17, this is now fixed
 */

#ifndef CPP_SOFT_ALIGNED_ARRAY_HPP
#define CPP_SOFT_ALIGNED_ARRAY_HPP

#include <array>

namespace cpp {

/*!
 * \brief Aligned std::array implementation
 * \tparam T The value type
 * \tparam S The size of the collection
 * \tparam A The alignment in bytes. This must be a positive power of two.
 *
 * If A needs to be other numbers than positive powers of two, aligned_array can
 * be used but with significantly more space overhead.
 */
template <typename T, std::size_t S, std::size_t A>
struct alignas(A) soft_aligned_array : std::array<T, S> {};

} //end of namespace cpp

#endif //CPP_SOFT_ALIGNED_ARRAY_HPP

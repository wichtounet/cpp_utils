//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains an aligned version of std::vector
 */

#ifndef CPP_ALIGNED_VECTOR_HPP
#define CPP_ALIGNED_VECTOR_HPP

#include "aligned_allocator.hpp"

namespace cpp {

template <typename T, std::size_t A>
using aligned_vector = std::vector<T, aligned_allocator<T, A>>;

} //end of namespace cpp

#endif //CPP_ARRAY_WRAPPER_HPP

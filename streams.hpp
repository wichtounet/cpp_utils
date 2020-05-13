//=======================================================================
// Copyright (c) 2013-2020 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file streams.hpp
 * \brief Contains extensions for streams. 
 *
 * The functions must be used with using so that they are considered when compiling. 
 */

#ifndef CPP_UTILS_STREAMS_HPP
#define CPP_UTILS_STREAMS_HPP

#include <iostream>
#include <vector>
#include <list>
#include <deque>

namespace cpp {

/*!
 * \brief operator<< overload to display the contents of a vector.
 * \param os The output stream
 * \param vec The vector to display
 * \tparam T The value type of the vector
 * \tparam A The allocator type of the vector
 * \return The output stream. 
 */
template <typename T, typename A>
std::ostream& operator<<(std::ostream& os, const std::vector<T, A>& vec) {
    os << "[";
    std::string comma;
    for (auto& v : vec) {
        os << comma << v;
        comma = ", ";
    }
    os << "]";
    return os;
}

/*!
 * \brief operator<< overload to display the contents of a list.
 * \param os The output stream
 * \param list The list to display
 * \tparam T The value type of the list
 * \tparam A The allocator type of the list
 * \return The output stream. 
 */
template <typename T, typename A>
std::ostream& operator<<(std::ostream& os, const std::list<T, A>& list) {
    os << "[";
    std::string comma;
    for (auto& v : list) {
        os << comma << v;
        comma = ", ";
    }
    os << "]";
    return os;
}

/*!
 * \brief operator<< overload to display the contents of a deque.
 * \param os The output stream
 * \param deq The deque to display
 * \tparam T The value type of the deque
 * \tparam A The allocator type of the deque
 * \return The output stream. 
 */
template <typename T, typename A>
std::ostream& operator<<(std::ostream& os, const std::deque<T, A>& deq) {
    os << "[";
    std::string comma;
    for (auto& v : deq) {
        os << comma << v;
        comma = ", ";
    }
    os << "]";
    return os;
}

} //end of the cpp namespace

#endif //CPP_UTILS_STREAMS_HPP

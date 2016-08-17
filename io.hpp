//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains functions to write to binary files
 */

#ifndef CPP_UTILS_IO_HPP
#define CPP_UTILS_IO_HPP

#include <iosfwd>

namespace cpp {

//Binary I/O utility functions

/*!
 * \brief Write the binary representation of the given value
 * \param os The stream to write to
 * \param v The value to write
 */
template <typename T>
void binary_write(std::ostream& os, const T& v) {
    os.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

/*!
 * \brief Write the binary representation of all the given values
 * \param os The stream to write to
 * \param c The container containing the values to write
 */
template <typename Container>
void binary_write_all(std::ostream& os, const Container& c) {
    for (auto& v : c) {
        binary_write(os, v);
    }
}

/*!
 * \brief Load the binary representation of the given value
 * \param os The stream to read from
 * \param c The value to read
 */
template <typename T>
void binary_load(std::istream& is, T& v) {
    is.read(reinterpret_cast<char*>(&v), sizeof(v));
}

/*!
 * \brief Load the binary representation of all the given values
 * \param os The stream to read from
 * \param c The container containing the values to read
 */
template <typename Container>
void binary_load_all(std::istream& is, Container& c) {
    for (auto& v : c) {
        binary_load(is, v);
    }
}

} //end of the cpp namespace

#endif //CPP_UTILS_IO_HPP

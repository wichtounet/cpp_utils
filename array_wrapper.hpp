//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains a simple wrapper over a raw array for size/begin/end/[]
 */

#ifndef CPP_ARRAY_WRAPPER_HPP
#define CPP_ARRAY_WRAPPER_HPP

namespace cpp {

template <typename T>
struct array_wrapper {
private:
    T* _memory;        ///< The array memory
    std::size_t _size; ///< The size of the array

public:
    using iterator       = T*;
    using const_iterator = const T*;

    array_wrapper(T* memory, std::size_t size)
            : _memory(memory), _size(size) {}

    /*
     * \brief Returns the size of the array
     */
    std::size_t size() const noexcept {
        return _size;
    }

    /*
     * \brief Returns a reference to the element at position i
     */
    T& operator[](std::size_t i) noexcept {
        return _memory[i];
    }

    /*
     * \brief Returns a reference to the element at position i
     */
    const T& operator[](std::size_t i) const noexcept {
        return _memory[i];
    }

    /*
     * \brief Returns an iterator to the first element of the array
     */
    iterator begin() noexcept {
        return &_memory[0];
    }

    /*
     * \brief Returns an iterator to the past-the-end element of the array
     */
    iterator end() noexcept {
        return &_memory[_size];
    }

    /*
     * \brief Returns an iterator to the first element of the array
     */
    const_iterator begin() const noexcept {
        return &_memory[0];
    }

    /*
     * \brief Returns an iterator to the past-the-end element of the array
     */
    const_iterator end() const noexcept {
        return &_memory[_size];
    }

    /*
     * \brief Returns an iterator to the first element of the array
     */
    const_iterator cbegin() const noexcept {
        return &_memory[0];
    }

    /*
     * \brief Returns an iterator to the past-the-end element of the array
     */
    const_iterator cend() const noexcept {
        return &_memory[_size];
    }
};

} //end of namespace cpp

#endif //CPP_ARRAY_WRAPPER_HPP

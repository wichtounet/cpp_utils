//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains an std::array-like implementation but with aligned data
 * access
 */

#ifndef CPP_ALIGNED_ARRAY_HPP
#define CPP_ALIGNED_ARRAY_HPP

namespace cpp {

/*!
 * \brief Aligned std::array implementation
 * \tparam T The value type
 * \tparam S The size of the collection
 * \tparam A The alignment in bytes
 */
template <typename T, std::size_t S, std::size_t A>
struct aligned_array {
    using value_type             = T;                                     ///< The type of value
    using size_type              = std::size_t;                           ///< The type of indices
    using difference_type        = std::ptrdiff_t;                        ///< The type of iterator difference
    using reference              = value_type&;                           ///< The type of a reference
    using const_reference        = value_type&;                           ///< The type of a const reference
    using pointer                = value_type*;                           ///< The type of a pointer
    using const_pointer          = const value_type*;                     ///< The type of a const pointer
    using iterator               = T*;                                    ///< The type of an iterator
    using const_iterator         = const T*;                              ///< The type of a const iterator
    using reverse_iterator       = std::reverse_iterator<iterator>;       ///< The type of a reverse iterator
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; ///< The type of a const reverse iterator

    /*!
     * \brief Initialize the array
     */
    aligned_array() {
        auto raw = reinterpret_cast<std::size_t>(storage);

        if (raw % A) {
            aligned_data = reinterpret_cast<value_type*>(raw + (A - (raw % A)));
        } else {
            aligned_data = reinterpret_cast<value_type*>(raw);
        }

        new (aligned_data) value_type[size()]();
    }

    /*!
     * \brief Copy construct an aligned_array
     */
    aligned_array(const aligned_array& rhs) noexcept {
        auto raw = reinterpret_cast<std::size_t>(storage);

        if (raw % A) {
            aligned_data = reinterpret_cast<value_type*>(raw + (A - (raw % A)));
        } else {
            aligned_data = reinterpret_cast<value_type*>(raw);
        }

        for(size_t i = 0; i < S; ++i){
            (*this)[i] = rhs[i];
        }
    }

    /*!
     * \brief Move construct an aligned_array
     */
    aligned_array(aligned_array&& rhs) noexcept {
        auto raw = reinterpret_cast<std::size_t>(storage);

        if (raw % A) {
            aligned_data = reinterpret_cast<value_type*>(raw + (A - (raw % A)));
        } else {
            aligned_data = reinterpret_cast<value_type*>(raw);
        }

        for(size_t i = 0; i < S; ++i){
            (*this)[i] = rhs[i];
        }
    }

    /*!
     * \brief Copy assign an aligned_array
     */
    aligned_array& operator=(const aligned_array& rhs) noexcept {
        if (this != &rhs) {
            for (size_t i = 0; i < S; ++i) {
                (*this)[i] = rhs[i];
            }
        }

        return *this;
    }

    /*!
     * \brief Move assign an aligned_array
     */
    aligned_array& operator=(aligned_array&& rhs) noexcept {
        if (this != &rhs) {
            for (size_t i = 0; i < S; ++i) {
                (*this)[i] = rhs[i];
            }
        }

        return *this;
    }

    reference at(std::size_t i) {
        if (i >= size()) {
            throw std::out_of_range("Out of bounds aligned_array");
        }

        return aligned_data[i];
    }

    const_reference at(std::size_t i) const {
        if (i >= size()) {
            throw std::out_of_range("Out of bounds aligned_array");
        }

        return aligned_data[i];
    }

    reference operator[](std::size_t i) noexcept {
        return aligned_data[i];
    }

    const_reference operator[](std::size_t i) const noexcept {
        return aligned_data[i];
    }

    reference front() noexcept {
        return aligned_data[0];
    }

    const_reference front() const noexcept {
        return aligned_data[0];
    }

    reference back() noexcept {
        return aligned_data[size() - 1];
    }

    const_reference back() const noexcept {
        return aligned_data[size() - 1];
    }

    T* data() noexcept {
        return aligned_data;
    }

    const T* data() const noexcept {
        return aligned_data;
    }

    iterator begin() {
        return &aligned_data[0];
    }

    const_iterator begin() const {
        return &aligned_data[0];
    }

    const_iterator cbegin() const {
        return &aligned_data[0];
    }

    iterator end() {
        return &aligned_data[size()];
    }

    const_iterator end() const {
        return &aligned_data[size()];
    }

    const_iterator cend() const {
        return &aligned_data[size()];
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(&aligned_data[size()]);
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(&aligned_data[size()]);
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(&aligned_data[size()]);
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(&aligned_data[0]);
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(&aligned_data[0]);
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(&aligned_data[0]);
    }

    constexpr bool empty() const noexcept {
        return !S;
    }

    constexpr std::size_t size() const noexcept {
        return S;
    }

    constexpr std::size_t max_size() const noexcept {
        return S;
    }

    void fill(const value_type& value) noexcept {
        for (std::size_t i = 0; i < size(); ++i) {
            aligned_data[i] = value;
        }
    }

    void swap(aligned_array& other){
        for(size_t i = 0; i < size(); ++i){
            using std::swap;
            swap((*this)[i], other[i]);
        }
    }

private:
    char storage[S * sizeof(T) + (A - 1)];
    T* aligned_data;
};

template <typename T, std::size_t N, std::size_t A>
void swap(aligned_array<T, N, A>& lhs, aligned_array<T, N, A>& rhs){
    lhs.swap(rhs);
}

} //end of namespace cpp

#endif //CPP_ARRAY_WRAPPER_HPP

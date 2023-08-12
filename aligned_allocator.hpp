//=======================================================================
// Copyright (c) 2013-2020 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains an allocator for aligned data
 */

#ifndef CPP_ALIGNED_ALLOCATOR_HPP
#define CPP_ALIGNED_ALLOCATOR_HPP

#include <stdexcept>
#include <cstdint>

namespace cpp {

/**
 * Allocator for aligned data.
 *
 * Modified from https://gist.github.com/donny-dont/1471329
 *
 * Modified from the Mallocator from Stephan T. Lavavej.
 * <http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx>
 */
template <typename T, std::size_t A>
struct aligned_allocator {
    using pointer         = T*;          ///< The pointer type
    using const_pointer   = const T*;    ///< The const pointer type
    using reference       = T&;          ///< The reference type
    using const_reference = const T&;    ///< The const reference type
    using value_type      = T;           ///< The value type
    using size_type       = std::size_t; ///< The size type
    using difference_type = ptrdiff_t;   ///< The difference type

    T* address(T& r) const {
        return &r;
    }

    const T* address(const T& s) const {
        return &s;
    }

    /*!
     *  \brief Return the maximum size allocated by this allocator
     *  \return the maximum size supported by this allocator
     */
    std::size_t max_size() const {
        // The following has been carefully written to be independent of
        // the definition of size_t and to avoid signed/unsigned warnings.
        return (static_cast<std::size_t>(0) - static_cast<std::size_t>(1)) / sizeof(T);
    }

    // The following must be the same for all allocators.
    template <typename U>
    struct rebind {
        using other = aligned_allocator<U, A>;
    };

    bool operator!=(const aligned_allocator& other) const {
        return !(*this == other);
    }

    void construct(T* const p, const T& t) const {
        void* const pv = static_cast<void*>(p);

        new (pv) T(t);
    }

    /*!
     * \brief Destroy a previously allocated object
     */
    void destroy(T* const p) const {
        p->~T();
    }

    // Returns true if and only if storage allocated from *this
    // can be deallocated from other, and vice versa.
    // Always returns true for stateless allocators.
    bool operator==(const aligned_allocator& /*other*/) const {
        return true;
    }

    aligned_allocator() = default;

    aligned_allocator(const aligned_allocator&)  = default;

    template <typename U>
    aligned_allocator(const aligned_allocator<U, A>&);

    ~aligned_allocator() = default;

    static T* aligned_allocate(std::size_t size) {
        auto required_bytes = sizeof(T) * size;
        auto offset         = (A - 1) + sizeof(uintptr_t);
        auto orig           = malloc(required_bytes + offset);

        if (!orig) {
            return nullptr;
        }

        if ((reinterpret_cast<size_t>(orig) + sizeof(uintptr_t)) % A == 0) {
            auto aligned = reinterpret_cast<void**>((reinterpret_cast<size_t>(orig) + sizeof(uintptr_t)));
            aligned[-1] = orig;
            return reinterpret_cast<T*>(aligned);
        } else {
            auto diff    = (reinterpret_cast<size_t>(orig) + sizeof(uintptr_t)) % A;
            auto offset  = A - diff;
            auto aligned = reinterpret_cast<void**>((reinterpret_cast<size_t>(orig) + sizeof(uintptr_t) + offset));
            aligned[-1] = orig;
            return reinterpret_cast<T*>(aligned);
        }
    }

    // The following will be different for each allocator.
    T* allocate(const std::size_t n) const {
        // The return value of allocate(0) is unspecified.
        // Mallocator returns nullptr in order to avoid depending
        // on malloc(0)'s implementation-defined behavior
        // (the implementation can define malloc(0) to return nullptr,
        // in which case the bad_alloc check below would fire).
        // All allocators can return nullptr in this case.
        if (n == 0) {
            return nullptr;
        }

        // All allocators should contain an integer overflow check.
        // The Standardization Committee recommends that std::length_error
        // be thrown in the case of integer overflow.
        if (n > max_size()) {
            throw std::length_error("aligned_allocator<T>::allocate() - Integer overflow.");
        }

        void* pv = aligned_allocate(n);

        // Allocators should throw std::bad_alloc in the case of memory allocation failure.
        if (!pv) {
            throw std::bad_alloc();
        }

        return static_cast<T*>(pv);
    }

    void deallocate(T* const ptr, const std::size_t /*n*/) const {
        //Note that the const_cast is only to allow compilation
        free((reinterpret_cast<void**>(const_cast<std::remove_const_t<T>*>(ptr)))[-1]);
    }

    // The following will be the same for all allocators that ignore hints.
    template <typename U>
    T* allocate(const std::size_t n, const U* /* const hint */) const {
        return allocate(n);
    }

    // Allocators are not required to be assignable, so
    // all allocators should have a private unimplemented
    // assignment operator. Note that this will trigger the
    // off-by-default (enabled under /Wall) warning C4626
    // "assignment operator could not be generated because a
    // base class assignment operator is inaccessible" within
    // the STL headers, but that warning is useless.
private:
    aligned_allocator& operator=(const aligned_allocator&);
};

} //end of namespace cpp

#endif //CPP_ARRAY_WRAPPER_HPP

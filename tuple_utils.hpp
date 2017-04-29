//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file tuple_utils.hpp
 * \brief Contains utilities to work with std::tuple
 */

#ifndef CPP_UTILS_TUPLE_HPP
#define CPP_UTILS_TUPLE_HPP

#include <tuple>

#include "cpp_utils/tmp.hpp"

namespace cpp {

namespace tuple_detail {

/*!
 * \brief Implementation o the functors for for_each
 */
template <size_t I, typename Tuple, typename F>
struct for_each_impl {
    /*!
     * \brief Apply the functor to each element of the tuple
     * \param t the tuple
     * \param f The functor
     */
    static void for_each(Tuple& t, F&& f) {
        for_each_impl<I - 1, Tuple, F>::for_each(t, std::forward<F>(f));
        f(std::get<I>(t));
    }

    /*!
     * \brief Apply the functor to each element of the tuple and its index
     * \param t the tuple
     * \param f The functor
     */
    static void for_each_i(Tuple& t, F&& f) {
        for_each_impl<I - 1, Tuple, F>::for_each_i(t, std::forward<F>(f));
        f(I, std::get<I>(t));
    }

    /*!
     * \brief Apply the functor to each element of the tuple and its constexpr index
     * \param t the tuple
     * \param f The functor
     */
    static void for_each_ix(Tuple& t, F&& f) {
        for_each_impl<I - 1, Tuple, F>::for_each_ix(t, std::forward<F>(f));
        f(std::integral_constant<size_t,I>(), std::get<I>(t));
    }

    /*!
     * \brief Apply the functor to each neighbouring pair of the tuple
     * \param t the tuple
     * \param f The functor
     */
    static void for_each_pair(Tuple& t, F&& f) {
        for_each_impl<I - 1, Tuple, F>::for_each_pair(t, std::forward<F>(f));
        f(std::get<I>(t), std::get<I + 1>(t));
    }

    /*!
     * \brief Apply the functor to each neighbouring pair of the tuple and its index
     * \param t the tuple
     * \param f The functor
     */
    static void for_each_pair_i(Tuple& t, F&& f) {
        for_each_impl<I - 1, Tuple, F>::for_each_pair_i(t, std::forward<F>(f));
        f(I, std::get<I>(t), std::get<I + 1>(t));
    }

    /*!
     * \brief Apply the functor to each neighbouring pair of the tuple and its index
     * \param t the tuple
     * \param f The functor
     */
    static void for_each_pair_ix(Tuple& t, F&& f) {
        for_each_impl<I - 1, Tuple, F>::for_each_pair_ix(t, std::forward<F>(f));
        f(std::integral_constant<size_t, I>(), std::get<I>(t), std::get<I + 1>(t));
    }

    /*!
     * \brief Apply the functor to each neighbouring pair of the tuple, from the end
     * \param t the tuple
     * \param f The functor
     */
    static void for_each_rpair(Tuple& t, F&& f) {
        f(std::get<I>(t), std::get<I + 1>(t));
        for_each_impl<I - 1, Tuple, F>::for_each_rpair(t, std::forward<F>(f));
    }

    /*!
     * \brief Apply the functor to each neighbouring pair of the tuple and its index, from the end
     * \param t the tuple
     * \param f The functor
     */
    static void for_each_rpair_i(Tuple& t, F&& f) {
        f(I, std::get<I>(t), std::get<I + 1>(t));
        for_each_impl<I - 1, Tuple, F>::for_each_rpair_i(t, std::forward<F>(f));
    }

    /*!
     * \brief Apply the functor to each neighbouring pair of the tuple and its index, from the end
     * \param t the tuple
     * \param f The functor
     */
    static void for_each_rpair_ix(Tuple& t, F&& f) {
        f(std::integral_constant<size_t, I>(), std::get<I>(t), std::get<I + 1>(t));
        for_each_impl<I - 1, Tuple, F>::for_each_rpair_ix(t, std::forward<F>(f));
    }
};

/*!
 * \copydoc for_each_impl
 */
template <typename Tuple, typename F>
struct for_each_impl<0, Tuple, F> {
    /*!
     * \copydoc for_each_impl::for_each
     */
    static void for_each(Tuple& t, F&& f) {
        f(std::get<0>(t));
    }

    /*!
     * \copydoc for_each_impl::for_each_i
     */
    static void for_each_i(Tuple& t, F&& f) {
        f(0, std::get<0>(t));
    }

    /*!
     * \copydoc for_each_impl::for_each_ix
     */
    static void for_each_ix(Tuple& t, F&& f) {
        f(std::integral_constant<size_t, 0>(), std::get<0>(t));
    }

    /*!
     * \copydoc for_each_impl::for_each_pair
     */
    static void for_each_pair(Tuple& t, F&& f) {
        f(std::get<0>(t), std::get<1>(t));
    }

    /*!
     * \copydoc for_each_impl::for_each_pair_i
     */
    static void for_each_pair_i(Tuple& t, F&& f) {
        f(0, std::get<0>(t), std::get<1>(t));
    }

    /*!
     * \copydoc for_each_impl::for_each_pair_ix
     */
    static void for_each_pair_ix(Tuple& t, F&& f) {
        f(std::integral_constant<size_t,0>(), std::get<0>(t), std::get<1>(t));
    }

    /*!
     * \copydoc for_each_impl::for_each_rpair
     */
    static void for_each_rpair(Tuple& t, F&& f) {
        f(std::get<0>(t), std::get<1>(t));
    }

    /*!
     * \copydoc for_each_impl::for_each_rpair_i
     */
    static void for_each_rpair_i(Tuple& t, F&& f) {
        f(0, std::get<0>(t), std::get<1>(t));
    }

    /*!
     * \copydoc for_each_impl::for_each_rpair_i
     */
    static void for_each_rpair_ix(Tuple& t, F&& f) {
        f(std::integral_constant<size_t,0>(), std::get<0>(t), std::get<1>(t));
    }
};

/*!
 * \brief Implementation of the functors for dual_for_each functions
 */
template <size_t I, typename Tuple1, typename Tuple2, typename F>
struct dual_for_each_impl {
    /*!
     * \brief Apply the functor to each element of both tuples, parallely
     * \param t1 The first tuple
     * \param t2 The second tuple
     * \param f The functor
     */
    static void for_each(Tuple1& t1, Tuple2& t2, F&& f) {
        dual_for_each_impl<I - 1, Tuple1, Tuple2, F>::for_each(t1, t2, std::forward<F>(f));
        f(std::get<I>(t1), std::get<I>(t2));
    }

    /*!
     * \brief Apply the functor to each element of both tuples and their index, parallely
     * \param t1 The first tuple
     * \param t2 The second tuple
     * \param f The functor
     */
    static void for_each_i(Tuple1& t1, Tuple2& t2, F&& f) {
        dual_for_each_impl<I - 1, Tuple1, Tuple2, F>::for_each_i(t1, t2, std::forward<F>(f));
        f(I, std::get<I>(t1), std::get<I>(t2));
    }

    /*!
     * \brief Apply the functor to each element of both tuples and their index, parallely
     * \param t1 The first tuple
     * \param t2 The second tuple
     * \param f The functor
     */
    static void for_each_ix(Tuple1& t1, Tuple2& t2, F&& f) {
        dual_for_each_impl<I - 1, Tuple1, Tuple2, F>::for_each_ix(t1, t2, std::forward<F>(f));
        f(std::integral_constant<size_t,I>(), std::get<I>(t1), std::get<I>(t2));
    }

    /*!
     * \brief Apply the functor to each pair of elements of both tuples, parallely
     * \param t1 The first tuple
     * \param t2 The second tuple
     * \param f The functor
     */
    static void for_each_pair(Tuple1& t1, Tuple2& t2, F&& f) {
        dual_for_each_impl<I - 1, Tuple1, Tuple2, F>::for_each_pair(t1, t2, std::forward<F>(f));
        f(std::get<I>(t1), std::get<I + 1>(t1), std::get<I>(t2), std::get<I + 1>(t2));
    }

    /*!
     * \brief Apply the functor to each pair of elements of both tuples and its index, in reverse order, parallely
     * \param t1 The first tuple
     * \param t2 The second tuple
     * \param f The functor
     */
    static void for_each_rpair_i(Tuple1& t1, Tuple2& t2, F&& f) {
        f(I, std::get<I>(t1), std::get<I + 1>(t1), std::get<I>(t2), std::get<I + 1>(t2));
        dual_for_each_impl<I - 1, Tuple1, Tuple2, F>::for_each_rpair_i(t1, t2, std::forward<F>(f));
    }

    /*!
     * \brief Apply the functor to each pair of elements of both tuples and its index, in reverse order, parallely
     * \param t1 The first tuple
     * \param t2 The second tuple
     * \param f The functor
     */
    static void for_each_rpair_ix(Tuple1& t1, Tuple2& t2, F&& f) {
        f(std::integral_constant<size_t,I>(), std::get<I>(t1), std::get<I + 1>(t1), std::get<I>(t2), std::get<I + 1>(t2));
        dual_for_each_impl<I - 1, Tuple1, Tuple2, F>::for_each_rpair_ix(t1, t2, std::forward<F>(f));
    }
};

/*!
 * \copydoc dual_for_each_impl
 */
template <typename Tuple1, typename Tuple2, typename F>
struct dual_for_each_impl<0, Tuple1, Tuple2, F> {
    /*!
     * \copydoc dual_for_each_impl::for_each
     */
    static void for_each(Tuple1& t1, Tuple2& t2, F&& f) {
        f(std::get<0>(t1), std::get<0>(t2));
    }

    /*!
     * \copydoc dual_for_each_impl::for_each_i
     */
    static void for_each_i(Tuple1& t1, Tuple2& t2, F&& f) {
        f(0, std::get<0>(t1), std::get<0>(t2));
    }

    /*!
     * \copydoc dual_for_each_impl::for_each_i
     */
    static void for_each_ix(Tuple1& t1, Tuple2& t2, F&& f) {
        f(std::integral_constant<size_t,0>(), std::get<0>(t1), std::get<0>(t2));
    }

    /*!
     * \copydoc dual_for_each_impl::for_each_pair
     */
    static void for_each_pair(Tuple1& t1, Tuple2& t2, F&& f) {
        f(std::get<0>(t1), std::get<1>(t1), std::get<0>(t2), std::get<1>(t2));
    }

    /*!
     * \copydoc dual_for_each_impl::for_each_rpair_i
     */
    static void for_each_rpair_i(Tuple1& t1, Tuple2& t2, F&& f) {
        f(0, std::get<0>(t1), std::get<1>(t1), std::get<0>(t2), std::get<1>(t2));
    }

    /*!
     * \copydoc dual_for_each_impl::for_each_rpair_i
     */
    static void for_each_rpair_ix(Tuple1& t1, Tuple2& t2, F&& f) {
        f(std::integral_constant<size_t,0>(), std::get<0>(t1), std::get<1>(t1), std::get<0>(t2), std::get<1>(t2));
    }
};

} //end of nanespace tuple_detail

// for_each

/*!
 * \brief Call the given functor with each element of the tuple
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 0))>
void for_each(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 1, Tuple, F>::for_each(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each element of the tuple
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 0))>
void for_each(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_i

/*!
 * \brief Call the given functor with each element of the tuple and its position (zero-indexing)
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 0))>
void for_each_i(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 1, Tuple, F>::for_each_i(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each element of the tuple and its position (zero-indexing)
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 0))>
void for_each_i(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_ix

/*!
 * \brief Call the given functor with each element of the tuple and its constexp position (zero-indexing)
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 0))>
void for_each_ix(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 1, Tuple, F>::for_each_ix(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each element of the tuple and its constexp position (zero-indexing)
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 0))>
void for_each_ix(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_pair

/*!
 * \brief Call the given functor with each contiguous pair of element of the tuple.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_pair(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 2, Tuple, F>::for_each_pair(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each contiguous pair of element of the tuple.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_pair(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_pair_i

/*!
 * \brief Call the given functor with each contiguous pair of element of the tuple and the position of the first element.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_pair_i(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 2, Tuple, F>::for_each_pair_i(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each contiguous pair of element of the tuple and the position of the first element.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_pair_i(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_pair_ix

/*!
 * \brief Call the given functor with each contiguous pair of element of the tuple and the position of the first element.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_pair_ix(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 2, Tuple, F>::for_each_pair_ix(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each contiguous pair of element of the tuple and the position of the first element.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_pair_ix(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_rpair

/*!
 * \brief Call the given functor with each contiguous pair of element of the tuple , in reverse order.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_rpair(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 2, Tuple, F>::for_each_rpair(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each contiguous pair of element of the tuple , in reverse order.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_rpair(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_rpair_i

/*!
 * \brief Call the given functor with each position of contiguous pair of element of the tuple, in reverse order.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_rpair_i(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 2, Tuple, F>::for_each_rpair_i(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each position of contiguous pair of element of the tuple, in reverse order.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_rpair_i(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_rpair_ix

/*!
 * \brief Call the given functor with each position of contiguous pair of element of the tuple, in reverse order.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_enable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_rpair_ix(Tuple& t, F&& f) {
    tuple_detail::for_each_impl<std::tuple_size<Tuple>::value - 2, Tuple, F>::for_each_rpair_ix(t, std::forward<F>(f));
}

/*!
 * \brief Call the given functor with each position of contiguous pair of element of the tuple, in reverse order.
 * \param t The tuple to iterate over
 * \param f The functor to use
 *
 * If the tuple is heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple, typename F, cpp_disable_if((std::tuple_size<Tuple>::value > 1))>
void for_each_rpair_ix(Tuple& t, F&& f) {
    cpp_unused(t);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each (t1, t2, f)

/*!
 * \brief Call the given function with elements of both tuples.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_enable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each(Tuple1& t1, Tuple2& t2, F&& f) {
    static_assert(std::tuple_size<Tuple1>::value == std::tuple_size<Tuple2>::value, "Can only iterate tuples of same size");

    tuple_detail::dual_for_each_impl<std::tuple_size<Tuple1>::value - 1, Tuple1, Tuple2, F>::for_each(t1, t2, std::forward<F>(f));
}

/*!
 * \brief Call the given function with elements of both tuples.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_disable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each(Tuple1& t1, Tuple2& t2, F&& f) {
    cpp_unused(t1);
    cpp_unused(t2);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_i (t1, t2, f)

/*!
 * \brief Call the given function with position of elements of both tuples.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_enable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each_i(Tuple1& t1, Tuple2& t2, F&& f) {
    static_assert(std::tuple_size<Tuple1>::value == std::tuple_size<Tuple2>::value, "Can only iterate tuples of same size");

    tuple_detail::dual_for_each_impl<std::tuple_size<Tuple1>::value - 1, Tuple1, Tuple2, F>::for_each_i(t1, t2, std::forward<F>(f));
}

/*!
 * \brief Call the given function with position of elements of both tuples.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_disable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each_i(Tuple1& t1, Tuple2& t2, F&& f) {
    cpp_unused(t1);
    cpp_unused(t2);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_ix (t1, t2, f)

/*!
 * \brief Call the given function with position of elements of both tuples.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_enable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each_ix(Tuple1& t1, Tuple2& t2, F&& f) {
    static_assert(std::tuple_size<Tuple1>::value == std::tuple_size<Tuple2>::value, "Can only iterate tuples of same size");

    tuple_detail::dual_for_each_impl<std::tuple_size<Tuple1>::value - 1, Tuple1, Tuple2, F>::for_each_ix(t1, t2, std::forward<F>(f));
}

/*!
 * \brief Call the given function with position of elements of both tuples.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_disable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each_ix(Tuple1& t1, Tuple2& t2, F&& f) {
    cpp_unused(t1);
    cpp_unused(t2);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_pair (t1, t2, f)

/*!
 * \brief Call the given function with pair of elements of both tuples.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_enable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each_pair(Tuple1& t1, Tuple2& t2, F&& f) {
    static_assert(std::tuple_size<Tuple1>::value == std::tuple_size<Tuple2>::value, "Can only iterate tuples of same size");

    if (std::tuple_size<Tuple1>::value > 1) {
        tuple_detail::dual_for_each_impl<std::tuple_size<Tuple1>::value - 2, Tuple1, Tuple2, F>::for_each_pair(t1, t2, std::forward<F>(f));
    }
}

/*!
 * \brief Call the given function with pair of elements of both tuples.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_disable_if((std::tuple_size<Tuple1>::value > 1 && std::tuple_size<Tuple2>::value > 1))>
void for_each_pair(Tuple1& t1, Tuple2& t2, F&& f) {
    cpp_unused(t1);
    cpp_unused(t2);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_rpair_i (t1, t2, f)

/*!
 * \brief Call the given function with position of pair of elements of both tuples, in reverse order.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_enable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each_rpair_i(Tuple1& t1, Tuple2& t2, F&& f) {
    static_assert(std::tuple_size<Tuple1>::value == std::tuple_size<Tuple2>::value, "Can only iterate tuples of same size");

    if (std::tuple_size<Tuple1>::value > 1) {
        tuple_detail::dual_for_each_impl<std::tuple_size<Tuple1>::value - 2, Tuple1, Tuple2, F>::for_each_rpair_i(t1, t2, std::forward<F>(f));
    }
}

/*!
 * \brief Call the given function with position of pair of elements of both tuples, in reverse order.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_disable_if((std::tuple_size<Tuple1>::value > 1 && std::tuple_size<Tuple2>::value > 1))>
void for_each_rpair_i(Tuple1& t1, Tuple2& t2, F&& f) {
    cpp_unused(t1);
    cpp_unused(t2);
    cpp_unused(f);

    // Nothing to iterate
}

// for_each_rpair_i (t1, t2, f)

/*!
 * \brief Call the given function with position of pair of elements of both tuples, in reverse order.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_enable_if((std::tuple_size<Tuple1>::value > 0 && std::tuple_size<Tuple2>::value > 0))>
void for_each_rpair_ix(Tuple1& t1, Tuple2& t2, F&& f) {
    static_assert(std::tuple_size<Tuple1>::value == std::tuple_size<Tuple2>::value, "Can only iterate tuples of same size");

    if (std::tuple_size<Tuple1>::value > 1) {
        tuple_detail::dual_for_each_impl<std::tuple_size<Tuple1>::value - 2, Tuple1, Tuple2, F>::for_each_rpair_ix(t1, t2, std::forward<F>(f));
    }
}

/*!
 * \brief Call the given function with position of pair of elements of both tuples, in reverse order.
 * \param t1 The first tuple to iterate
 * \param t2 The second tuple to iterate
 * \param f The functor to use
 *
 * If the tuples are heterogeneous, the functor must be generic (i.e. generic lambda)
 */
template <typename Tuple1, typename Tuple2, typename F, cpp_disable_if((std::tuple_size<Tuple1>::value > 1 && std::tuple_size<Tuple2>::value > 1))>
void for_each_rpair_ix(Tuple1& t1, Tuple2& t2, F&& f) {
    cpp_unused(t1);
    cpp_unused(t2);
    cpp_unused(f);

    // Nothing to iterate
}

} //end of namespace cpp

#endif //CPP_UTILS_TUPLE_HPP

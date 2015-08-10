//=======================================================================
// Copyright (c) 2013-2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file data.hpp
 * \brief Contains basic data manipulation functions.
 */

#ifndef CPP_UTILS_DATA_HPP
#define CPP_UTILS_DATA_HPP

#include <numeric>  //for std::accumulate
#include <cmath>    //for std::sqrt

#include "assert.hpp"

/*!
 * \namespace cpp
 * \brief cpp_utils main namespace
 */
namespace cpp {

/*!
 * \brief Compute the mean of values of the given range
 * \param first Start of the range
 * \param last End of the range
 * \return the mean of the values of the given range
 */
template<typename Iterator>
double mean(Iterator first, Iterator last){
    return std::accumulate(first, last, 0.0) / std::distance(first, last);
}

/*!
 * \brief Compute the mean of values in the given container.
 * \param container The container to compute the mean from.
 * \return the mean of the values in the given container.
 */
template<typename Container>
double mean(const Container& container){
    return mean(std::begin(container), std::end(container));
}

/*!
 * \brief Compute the standard deviation of values of the given range
 * \param first Start of the range
 * \param last End of the range
 * \param mean The mean of the range
 * \return the standard deviation of the values of the given range
 */
template<typename Iterator>
double stddev(Iterator first, Iterator last, double mean){
    double std = 0.0;
    for(auto it = first; it != last; ++it){
        std += (*it - mean) * (*it - mean);
    }
    return std::sqrt(std / std::distance(first, last));
}

/*!
 * \brief Compute the standard deviation of values in the given container.
 * \param container The container to compute the mean from.
 * \param mean The mean of the range
 * \return the standard deviation of the values in the given container.
 */
template<typename Container>
double stddev(const Container& container, double mean){
    return stddev(std::begin(container), std::end(container), mean);
}

/*!
 * \brief Normalize all the values of the container
 * \param container The container to normalize
 *
 * The values are normalized so the range has zero-mean and unit-variance.
 */
template<typename Container>
void normalize(Container& container){
    //normalize to zero-mean
    auto m = mean(container);
    for(auto& v : container){
        v -= m;
    }

    //normalize to unit variance
    auto s = stddev(container, 0.0);

    if(s != 0.0){
        for(auto& v : container){
            v /= s;
        }
    }
}

/*!
 * \brief Normalize each value contained in the given range.
 * \param first Start of the range
 * \param last End of the range
 *
 * The values are normalized so the range has zero-mean and unit-variance.
 */
template<typename Iterator>
void normalize_each(Iterator first, Iterator last){
    for(; first != last; ++first){
        normalize(*first);
    }
}

/*!
 * \brief Normalize each value contained in the given range.
 * \param container The container holding the ranges to normalize.
 *
 * The values are normalized so the range has zero-mean and unit-variance.
 */
template<typename Container>
void normalize_each(Container& container){
    normalize_each(std::begin(container), std::end(container));
}

} //end of the cpp namespace

#endif //CPP_UTILS_ALGORITHM_HPP

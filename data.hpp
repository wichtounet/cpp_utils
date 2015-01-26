//=======================================================================
// Copyright (c) 2013-2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_DATA_HPP
#define CPP_UTILS_DATA_HPP

#include <numeric>  //for std::accumulate
#include <cmath>    //for std::sqrt

#include "assert.hpp"

namespace cpp {

template<typename Iterator>
double mean(Iterator first, Iterator last){
    return std::accumulate(first, last, 0.0) / std::distance(first, last);
}

template<typename Container>
double mean(const Container& container){
    return mean(std::begin(container), std::end(container));
}

template<typename Iterator>
double stddev(Iterator first, Iterator last, double mean){
    double std = 0.0;
    for(auto it = first; it != last; ++it){
        std += (*it - mean) * (*it - mean);
    }
    return std::sqrt(std / std::distance(first, last));
}

template<typename Container>
double stddev(const Container& container, double mean){
    return stddev(std::begin(container), std::end(container), mean);
}

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

template<typename Iterator>
void normalize_each(Iterator first, Iterator last){
    for(; first != last; ++first){
        normalize(*first);
    }
}

template<typename Container>
void normalize_each(Container& values){
    normalize_each(std::begin(values), std::end(values));
}

} //end of the cpp namespace

#endif //CPP_UTILS_ALGORITHM_HPP

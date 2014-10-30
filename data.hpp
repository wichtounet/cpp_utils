//=======================================================================
// Copyright (c) 2013-2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_DATA_HPP
#define CPP_UTILS_DATA_HPP

#include "assert.hpp"

namespace cpp {

template<typename Iterator>
double mean(Iterator first, Iterator last){
    double mean = 0.0;
    for(; first != last; ++first){
        mean += *first;
    }
    return mean / std::distance(first, last);
}

template<typename Container>
double mean(const Container& container){
    return mean(std::begin(container), std::end(container));
}

template<typename Iterator>
double stddev(Iterator first, Iterator last, double mean){
    double std = 0.0;
    for(; first != last; ++first){
        std += (*first - mean) * (*first - mean);
    }
    return std::sqrt(std / std::distance(first, last));
}

template<typename Container>
double stddev(const Container& container, double mean){
    return stddev(std::begin(container), std::end(container), mean);
}

template<typename Iterator>
void normalize_each(Iterator first, Iterator last){
    for(; first != last; ++first){
        //zero-mean
        auto m = mean(*first);
        for(auto& v : *first){
            v -= m;
        }
        //unit variance
        auto s = stddev(*first, 0.0);
        for(auto& v : *first){
            v /= s;
        }
    }
}

template<typename Container>
void normalize_each(Container& values){
    normalize_each(std::begin(values), std::end(values));
}

} //end of the cpp namespace

#endif //CPP_UTILS_ALGORITHM_HPP

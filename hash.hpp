//=======================================================================
// Copyright (c) 2013-2020 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file hash.hpp
 * \brief Contains hashing functions.
 */

#ifndef CPP_UTILS_HASH_HPP
#define CPP_UTILS_HASH_HPP

#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace cpp {

// Transparent string hash for unordered containers
struct string_hash {
    using is_transparent = void;

    [[nodiscard]] size_t operator()(const char* str) const {
        return std::hash<std::string_view>{}(str);
    }

    [[nodiscard]] size_t operator()(std::string_view str) const {
        return std::hash<std::string_view>{}(str);
    }

    [[nodiscard]] size_t operator()(const std::string& str) const {
        return std::hash<std::string>{}(str);
    }
};

using string_hash_set = std::unordered_set<std::string, string_hash, std::equal_to<>>;

template <typename Value>
using string_hash_map = std::unordered_map<std::string, Value, string_hash, std::equal_to<>>;

} // namespace cpp

#endif //CPP_UTILS_HASH_HPP

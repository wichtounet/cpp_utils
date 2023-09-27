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

#include <ranges>
#include <algorithm>
#include <cstdint>
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

// Transparent case-ignoring string hash for unordered containers
struct istring_hash {
    using is_transparent = void;

    static constexpr size_t nocase_hash(const std::string_view sv) {
        const size_t prime = 0x1000193;
        size_t value  = 0x811c9dc5;

        for (const unsigned char c : sv) {
            value = value ^ static_cast<uint8_t>(std::tolower(c));
            value *= prime;
        }

        return value;
    }

    [[nodiscard]] size_t operator()(const char* str) const {
        return nocase_hash(str);
    }

    [[nodiscard]] size_t operator()(std::string_view str) const {
        return nocase_hash(str);
    }

    [[nodiscard]] size_t operator()(const std::string& str) const {
        return nocase_hash(str);
    }
};

struct istring_compare {
    using is_transparent = void;

    static bool ichar_equals(char a, char b) {
        return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
    }

    bool operator()(std::string_view lhs, std::string_view rhs) const {
        return std::ranges::equal(lhs, rhs, ichar_equals);
    }
};

using string_hash_set = std::unordered_set<std::string, string_hash, std::equal_to<>>;
using istring_hash_set = std::unordered_set<std::string, istring_hash, istring_compare>;

template <typename Value>
using string_hash_map = std::unordered_map<std::string, Value, string_hash, std::equal_to<>>;

template <typename Value>
using istring_hash_map = std::unordered_map<std::string, Value, istring_hash, istring_compare>;

} // namespace cpp

#endif //CPP_UTILS_HASH_HPP

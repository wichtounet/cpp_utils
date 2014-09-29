//=======================================================================
// Copyright (c) 2013-2014 Baptiste Wicht.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_TMP_HPP
#define CPP_UTILS_TMP_HPP

#define HAS_MEM_FUNC(func, name)                                              \
template<typename T, typename Sign>                                           \
struct name {                                                                 \
    typedef char yes[1];                                                      \
    typedef char no [2];                                                      \
    template <typename U, U> struct type_check;                               \
    template <typename _1> static yes &chk(type_check<Sign, &_1::func> *);    \
    template <typename   > static no  &chk(...);                              \
    static bool constexpr const value = sizeof(chk<T>(0)) == sizeof(yes);     \
}

#define HAS_STATIC_FIELD(field, name)                                                       \
template <class T>                                                                          \
class name {                                                                                \
    template<typename U, typename =                                                         \
        typename std::enable_if<!std::is_member_pointer<decltype(&U::field)>::value>::type> \
    static std::true_type check(int);                                                       \
    template <typename>                                                                     \
    static std::false_type check(...);                                                      \
public:                                                                                     \
    static constexpr const bool value = decltype(check<T>(0))::value;                       \
};

#endif //CPP_UTILS_TMP_HPP
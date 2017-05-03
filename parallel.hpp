//=======================================================================
// Copyright (c) 2013-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.  // (See accompanying file LICENSE or copy at //  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains parallel algorithms implementations.
 */

#ifndef CPP_UTILS_PARALLEL_HPP
#define CPP_UTILS_PARALLEL_HPP

#ifdef CPP_UTILS_NO_EXCEPT
#include <iostream>
#endif

#include <thread>
#include <future>
#include <vector>
#include <functional>
#include <algorithm>

#include "assert.hpp"
#include "tmp.hpp"

namespace cpp {

/*!
 * \brief Call the given functor after having taken the given lock.
 *
 * The lock will remain locked for the duration of the functor.
 * \param lock The lock to lock
 * \param fun The functor to call.
 */
template <typename Lock, typename Functor>
void with_lock(Lock& lock, Functor fun) {
    std::unique_lock<Lock> l(lock);
    fun();
}

//1. Normal versions (no thread pool)

/*!
 * \brief Applies the given functor, concurrently, to the result of dereferencing every iterator in the range [first, last).
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void parallel_foreach(Iterator first, Iterator last, Functor fun) {
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for (; first != last; ++first) {
        futures.push_back(std::move(std::async(std::launch::async, fun, *first)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

/*!
 * \brief Applies the given functor, concurrently, to each value in the given container.
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param container The container to iterate.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void parallel_foreach(Container& container, Functor fun) {
    std::vector<std::future<void>> futures;
    futures.reserve(container.size());

    for (std::size_t i = 0; i < container.size(); ++i) {
        futures.push_back(std::move(std::async(std::launch::async, fun, container[i])));
    }

    //No need to wait for the futures, the destructor will do it for us
}

/*!
 * \brief Applies the given functor, concurrently, to the result of dereferencing every iterator in the range [first, last) and its position in the range. .
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void parallel_foreach_i(Iterator first, Iterator last, Functor fun) {
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for (; first != last; ++first) {
        futures.push_back(std::move(std::async(std::launch::async, fun, *first, futures.size())));
    }

    //No need to wait for the futures, the destructor will do it for us
}

/*!
 * \brief Applies the given functor, concurrently, to every element in the given container and its position in the container. .
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param container The container to iterate.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void parallel_foreach_i(Container& container, Functor fun) {
    std::vector<std::future<void>> futures;
    futures.reserve(container.size());

    for (std::size_t i = 0; i < container.size(); ++i) {
        futures.push_back(std::move(std::async(std::launch::async, fun, container[i], i)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

/*!
 * \brief Applies the given functor, concurrently, to each iterator in the range [first, last).
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void parallel_foreach_it(Iterator first, Iterator last, Functor fun) {
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for (; first != last; ++first) {
        futures.push_back(std::move(std::async(std::launch::async, fun, first)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

/*!
 * \brief Applies the given functor, concurrently, to each iterator in the given container.
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param container The container to iterate.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void parallel_foreach_it(Container& container, Functor fun) {
    using std::begin;
    using std::end;

    parallel_foreach_it(begin(container), end(container), fun);
}

/*!
 * \brief Applies the given functor, concurrently, to each value in the range [first, last).
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Functor>
void parallel_foreach_n(std::size_t first, std::size_t last, Functor fun) {
    std::vector<std::future<void>> futures;
    futures.reserve(last - first);

    for (std::size_t i = first; i != last; ++i) {
        futures.push_back(std::move(std::async(std::launch::async, fun, i)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

/*!
 * \brief Applies the given functor, concurrently, to each index in the given range [first, last).
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename Iterator, typename Functor>
void parallel_foreach_i_only(Iterator first, Iterator last, Functor fun) {
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for (; first != last; ++first) {
        futures.push_back(std::move(std::async(std::launch::async, fun, futures.size())));
    }

    //No need to wait for the futures, the destructor will do it for us
}

/*!
 * \brief Applies the given functor, concurrently, to each index in the given container.
 *
 * The number of threads used is chosen by the STL. The processing order is undefined.
 *
 * \param container The container to iterate.
 * \param fun The functor to apply.
 */
template <typename Container, typename Functor>
void parallel_foreach_i_only(Container& container, Functor fun) {
    parallel_foreach_n(0, container.size(), fun);
}

///////////////////////////
//2. Thread pool versions//
///////////////////////////

/*!
 * \brief Applies the given functor, concurrently, to the result of dereferencing every iterator in the range [first, last).
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename TP, typename Iterator, typename Functor,
          cpp_enable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach(TP& thread_pool, Iterator first, Iterator last, Functor fun) {
    auto n    = std::distance(first, last);
    auto part = n / thread_pool.size();

    if (part < 2) {
        for (; first != last; ++first) {
            thread_pool.do_task(fun, *first);
        }
    } else {
        auto batch_functor = [fun](Iterator first, Iterator last) {
            for (Iterator it = first; it != last; ++it) {
                fun(*it);
            }
        };

        //Distribute evenly the batches

        for (std::size_t t = 0; t < thread_pool.size(); ++t) {
            thread_pool.do_task(batch_functor, first + t * part, first + (t + 1) * part);
        }

        //Distribute the remainders

        auto rem = n % thread_pool.size();
        if (rem > 0) {
            for (Iterator it = last - rem; it < last; ++it) {
                thread_pool.do_task(fun, *it);
            }
        }
    }

    thread_pool.wait();
}

/*!
 * \brief Applies the given functor, concurrently, to the result of dereferencing every iterator in the range [first, last).
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename TP, typename Iterator, typename Functor,
          cpp_disable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach(TP& thread_pool, Iterator first, Iterator last, Functor fun) {
    for (; first != last; ++first) {
        thread_pool.do_task(fun, *first);
    }

    thread_pool.wait();
}

/*!
 * \brief Applies the given functor, concurrently, to each value in the given container.
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param container The container to iterate through.
 * \param fun The functor to apply.
 */
template <typename TP, typename Container, typename Functor>
void parallel_foreach(TP& thread_pool, Container& container, Functor fun) {
    using std::begin;
    using std::end;
    parallel_foreach(thread_pool, begin(container), end(container), fun);
}

/*!
 * \brief Applies the given functor, concurrently, to the result of dereferencing every iterator in the range [first, last) and index.
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename TP, typename Iterator, typename Functor,
          cpp_enable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach_i(TP& thread_pool, Iterator first, Iterator last, Functor fun) {
    auto n    = std::distance(first, last);
    auto part = n / thread_pool.size();

    if (part < 2) {
        for (std::size_t i = 0; first != last; ++first, ++i) {
            thread_pool.do_task(fun, *first, i);
        }
    } else {
        auto batch_functor = [fun](Iterator first, Iterator last, std::size_t i_start) {
            std::size_t i = i_start;
            for (Iterator it = first; it != last; ++it, ++i) {
                fun(*it, i);
            }
        };

        //Distribute evenly the batches

        for (std::size_t t = 0; t < thread_pool.size(); ++t) {
            thread_pool.do_task(batch_functor, first + t * part, first + (t + 1) * part, t * part);
        }

        //Distribute the remainders

        auto rem = n % thread_pool.size();
        if (rem > 0) {
            std::size_t i = n - rem;
            for (Iterator it = last - rem; it < last; ++it, ++i) {
                thread_pool.do_task(fun, *it, i);
            }
        }
    }

    thread_pool.wait();
}

/*!
 * \brief Applies the given functor, concurrently, to the result of dereferencing every iterator in the range [first, last) and index.
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename TP, typename Iterator, typename Functor,
          cpp_disable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach_i(TP& thread_pool, Iterator first, Iterator last, Functor fun) {
    for (std::size_t i = 0; first != last; ++first, ++i) {
        thread_pool.do_task(fun, *first, i);
    }

    thread_pool.wait();
}

/*!
 * \brief Applies the given functor, concurrently, to each value in the container and its index.
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param container the container to iterate through
 * \param fun The functor to apply.
 */
template <typename TP, typename Container, typename Functor>
void parallel_foreach_i(TP& thread_pool, Container& container, Functor fun) {
    using std::begin;
    using std::end;
    parallel_foreach_i(thread_pool, begin(container), end(container), fun);
}

/*!
 * \brief Applies the given functor, concurrently, to each iterator in the range [first, last).
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename TP, typename Iterator, typename Functor,
          cpp_enable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach_it(TP& thread_pool, Iterator first, Iterator last, Functor fun) {
    auto n    = std::distance(first, last);
    auto part = n / thread_pool.size();

    if (part < 2) {
        for (; first != last; ++first) {
            thread_pool.do_task(fun, first);
        }
    } else {
        auto batch_functor = [fun](Iterator first, Iterator last) {
            for (Iterator it = first; it != last; ++it) {
                fun(it);
            }
        };

        //Distribute evenly the batches

        for (std::size_t t = 0; t < thread_pool.size(); ++t) {
            thread_pool.do_task(batch_functor, first + t * part, first + (t + 1) * part);
        }

        //Distribute the remainders

        auto rem = n % thread_pool.size();
        if (rem > 0) {
            for (Iterator it = last - rem; it < last; ++it) {
                thread_pool.do_task(fun, it);
            }
        }
    }

    thread_pool.wait();
}

/*!
 * \brief Applies the given functor, concurrently, to each iterator in the range [first, last).
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename TP, typename Iterator, typename Functor,
          cpp_disable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach_it(TP& thread_pool, Iterator first, Iterator last, Functor fun) {
    for (; first != last; ++first) {
        thread_pool.do_task(fun, first);
    }

    thread_pool.wait();
}

/*!
 * \brief Applies the given functor, concurrently, to each iterator in the given container.
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param container The container to iterate through.
 * \param fun The functor to apply.
 */
template <typename TP, typename Container, typename Functor>
void parallel_foreach_it(TP& thread_pool, Container& container, Functor fun) {
    using std::begin;
    using std::end;

    parallel_foreach_it(thread_pool, begin(container), end(container), fun);
}

/*!
 * \brief Applies the given functor, concurrently, to each index in the range [first, last).
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename TP, typename Iterator, typename Functor>
void parallel_foreach_i_only(TP& thread_pool, Iterator first, Iterator last, Functor fun) {
    if (std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value) {
        parallel_foreach_n(thread_pool, 0, std::distance(first, last), fun);
    } else {
        for (std::size_t i = 0; first != last; ++first, ++i) {
            thread_pool.do_task(fun, i);
        }

        thread_pool.wait();
    }
}

/*!
 * \brief Applies the given functor, concurrently, to each index in the container.
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param container The container to iterate through.
 * \param fun The functor to apply.
 */
template <typename TP, typename Container, typename Functor>
void parallel_foreach_i_only(TP& thread_pool, Container& container, Functor fun) {
    parallel_foreach_n(thread_pool, 0, container.size(), fun);
}

/*!
 * \brief Applies the given functor, concurrently, to each index in the range [first, last).
 *
 * All the jobs are submitted to the given thread pool.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param first The beginning of the range
 * \param last The end of the range
 * \param fun The functor to apply.
 */
template <typename TP, typename Functor>
void parallel_foreach_n(TP& thread_pool, std::size_t first, std::size_t last, Functor fun) {
    auto n    = last - first;
    auto part = n / thread_pool.size();

    if (part < 2) {
        for (std::size_t i = first; i < last; ++i) {
            thread_pool.do_task(fun, i);
        }
    } else {
        auto batch_functor = [fun](std::size_t first, std::size_t last) {
            for (std::size_t i = first; i < last; ++i) {
                fun(i);
            }
        };

        //Distribute evenly the batches

        for (std::size_t t = 0; t < thread_pool.size(); ++t) {
            thread_pool.do_task(batch_functor, t * part, (t + 1) * part);
        }

        //Distribute the remainders

        auto rem = n % thread_pool.size();
        if (rem > 0) {
            for (std::size_t i = last - rem; i < last; ++i) {
                thread_pool.do_task(fun, i);
            }
        }
    }

    thread_pool.wait();
}

/*!
 * \brief Applies the given functor, concurrently, to each value in the range [f_first, f_last) and the range [s_first, s_last).
 *
 * All the jobs are submitted to the given thread pool.
 * Both range are iterated at the same speed, one by one.
 *
 * \param thread_pool The thread pool responsible for scheduling the jobs.
 * \param f_first The beginning of the first range
 * \param f_last The end of the first range
 * \param s_first The beginning of the second range
 * \param s_last The end of the second range
 * \param fun The functor to apply.
 */
template <typename TP, typename Iterator, typename Iterator2, typename Functor>
void parallel_foreach_pair_i(TP& thread_pool, Iterator f_first, Iterator f_last, Iterator2 s_first, Iterator2 s_last, Functor fun) {
    cpp_unused(s_last);

    if (std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value) {
        const std::size_t n    = std::distance(f_first, f_last);
        const std::size_t t    = thread_pool.size();
        const std::size_t part = n / t;

        auto batch_functor = [fun, f_first, s_first](std::size_t first, std::size_t last) {
            for (std::size_t i = first; i < last; ++i) {
                fun(*(std::next(f_first, i)), *(std::next(s_first, i)), i);
            }
        };

        //Distribute evenly the batches

        for (std::size_t i = 0; i < t; ++i) {
            thread_pool.do_task(batch_functor, i * part, (i + 1) * part);
        }

        //Distribute the remainders

        auto rem = n % t;
        if (rem > 0) {
            for (std::size_t i = n - rem; i < n; ++i) {
                thread_pool.do_task(fun, *(std::next(f_first, i)), *(std::next(s_first, i)), i);
            }
        }
    } else {
        for (std::size_t i = 0; f_first != f_last; ++f_first, ++s_first, ++i) {
            thread_pool.do_task(fun, *f_first, *s_first, i);
        }
    }

    thread_pool.wait();
}

} //end of the cpp namespace

#include "thread_pool.hpp"

#endif //CPP_UTILS_PARALLEL_HPP

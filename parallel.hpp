//=======================================================================
// Copyright (c) 2013-2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_PARALLEL_HPP
#define CPP_UTILS_PARALLEL_HPP

#ifdef CPP_UTILS_NO_EXCEPT
#include <iostream>
#endif

#include<thread>
#include<future>
#include<vector>
#include<deque>
#include<functional>
#include<algorithm>

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
template<typename Lock, typename Functor>
void with_lock(Lock& lock, Functor fun){
    std::unique_lock<Lock> l(lock);
    fun();
}

//1. Normal versions (no thread pool)

//Parallel for_each giving the elements to the functor

template<typename Iterator, typename Functor>
void parallel_foreach(Iterator first, Iterator last, Functor fun){
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for(; first != last; ++first){
        futures.push_back(std::move(std::async(std::launch::async, fun, *first)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Container, typename Functor>
void parallel_foreach(Container& container, Functor fun){
    std::vector<std::future<void>> futures;
    futures.reserve(container.size());

    for(std::size_t i = 0; i < container.size(); ++i){
        futures.push_back(std::move(std::async(std::launch::async, fun, container[i])));
    }

    //No need to wait for the futures, the destructor will do it for us
}

//Parallel for_each giving the elements and their indices to the functor

template<typename Iterator, typename Functor>
void parallel_foreach_i(Iterator first, Iterator last, Functor fun){
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for(; first != last; ++first){
        futures.push_back(std::move(std::async(std::launch::async, fun, *first, futures.size())));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Container, typename Functor>
void parallel_foreach_i(Container& container, Functor fun){
    std::vector<std::future<void>> futures;
    futures.reserve(container.size());

    for(std::size_t i = 0; i < container.size(); ++i){
        futures.push_back(std::move(std::async(std::launch::async, fun, container[i], i)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

//Parallel for_each giving the iterators to the functor

template<typename Iterator, typename Functor>
void parallel_foreach_it(Iterator first, Iterator last, Functor fun){
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for(; first != last; ++first){
        futures.push_back(std::move(std::async(std::launch::async, fun, first)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Container, typename Functor>
void parallel_foreach_it(Container& container, Functor fun){
    using std::begin;
    using std::end;

    parallel_foreach_it(begin(container), end(container), fun);
}

// Parallel for_each giving only the index to the functor

template<typename Iterator, typename Functor>
void parallel_foreach_i_only(Iterator first, Iterator last, Functor fun){
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for(; first != last; ++first){
        futures.push_back(std::move(std::async(std::launch::async, fun, futures.size())));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Container, typename Functor>
void parallel_foreach_i_only(Container& container, Functor fun){
    std::vector<std::future<void>> futures;
    futures.reserve(container.size());

    for(std::size_t i = 0; i < container.size(); ++i){
        futures.push_back(std::move(std::async(std::launch::async, fun, i)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

// Parallel for_each on a range

template<typename Functor>
void parallel_foreach_n(std::size_t first, std::size_t last, Functor fun){
    std::vector<std::future<void>> futures;
    futures.reserve(last - first);

    for(std::size_t i = first; i != last; ++i){
        futures.push_back(std::move(std::async(std::launch::async, fun, i)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

///////////////////////////
//2. Thread pool versions//
///////////////////////////

// Parallel for_each giving the element to the functor

template<typename TP, typename Iterator, typename Functor,
    cpp_enable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach(TP& thread_pool, Iterator first, Iterator last, Functor fun){
    auto n = std::distance(first, last);;
    auto part = n / thread_pool.size();

    if(part < 2){
        for(; first != last; ++first){
            thread_pool.do_task(fun, *first);
        }
    } else {
        auto batch_functor = [fun](Iterator first, Iterator last){
            for(Iterator it = first; it != last; ++it){
                fun(*it);
            }
        };

        //Distribute evenly the batches

        for(Iterator it = first; it + part < last; it += part){
            thread_pool.do_task(batch_functor, it, it + part);
        }

        //Distribute the remainders

        auto rem = n % thread_pool.size();
        if(rem > 0){
            for(Iterator it = last - rem; it < last; ++it){
                thread_pool.do_task(fun, *it);
            }
        }
    }

    thread_pool.wait();
}

template<typename TP, typename Iterator, typename Functor,
    cpp_disable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach(TP& thread_pool, Iterator first, Iterator last, Functor fun){
    for(; first != last; ++first){
        thread_pool.do_task(fun, *first);
    }

    thread_pool.wait();
}

template<typename TP, typename Container, typename Functor>
void parallel_foreach(TP& thread_pool, Container& container, Functor fun){
    using std::begin;
    using std::end;
    parallel_foreach(thread_pool, begin(container), end(container), fun);
}

// Parallel for_each giving the element and the index to the functor

template<typename TP, typename Iterator, typename Functor,
    cpp_enable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach_i(TP& thread_pool, Iterator first, Iterator last, Functor fun){
    auto n = std::distance(first, last);;
    auto part = n / thread_pool.size();

    if(part < 2){
        for(std::size_t i = 0; first != last; ++first, ++i){
            thread_pool.do_task(fun, *first, i);
        }
    } else {
        auto batch_functor = [fun](Iterator first, Iterator last, std::size_t i_start){
            std::size_t i = i_start;
            for(Iterator it = first; it != last; ++it, ++i){
                fun(*it, i);
            }
        };

        //Distribute evenly the batches

        std::size_t i = 0;
        for(Iterator it = first; it + part < last; it += part, i += part){
            thread_pool.do_task(batch_functor, it, it + part, i);
        }

        //Distribute the remainders

        auto rem = n % thread_pool.size();
        if(rem > 0){
            i = n - rem;
            for(Iterator it = last - rem; it < last; ++it, ++i){
                thread_pool.do_task(fun, *it, i);
            }
        }
    }

    thread_pool.wait();

    thread_pool.wait();
}

template<typename TP, typename Iterator, typename Functor,
    cpp_disable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach_i(TP& thread_pool, Iterator first, Iterator last, Functor fun){
    for(std::size_t i = 0; first != last; ++first, ++i){
        thread_pool.do_task(fun, *first, i);
    }

    thread_pool.wait();
}

template<typename TP, typename Container, typename Functor>
void parallel_foreach_i(TP& thread_pool, Container& container, Functor fun){
    using std::begin;
    using std::end;
    parallel_foreach_i(thread_pool, begin(container), end(container), fun);
}

// Parallel for_each giving the iterator to the functor

template<typename TP, typename Iterator, typename Functor,
    cpp_enable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach_it(TP& thread_pool, Iterator first, Iterator last, Functor fun){
    auto n = std::distance(first, last);;
    auto part = n / thread_pool.size();

    if(part < 2){
        for(; first != last; ++first){
            thread_pool.do_task(fun, first);
        }
    } else {
        auto batch_functor = [fun](Iterator first, Iterator last){
            for(Iterator it = first; it != last; ++it){
                fun(it);
            }
        };

        //Distribute evenly the batches

        for(Iterator it = first; it + part < last; it += part){
            thread_pool.do_task(batch_functor, it, it + part);
        }

        //Distribute the remainders

        auto rem = n % thread_pool.size();
        if(rem > 0){
            for(Iterator it = last - rem; it < last; ++it){
                thread_pool.do_task(fun, it);
            }
        }
    }

    thread_pool.wait();
}

template<typename TP, typename Iterator, typename Functor,
    cpp_disable_if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value)>
void parallel_foreach_it(TP& thread_pool, Iterator first, Iterator last, Functor fun){
    for(; first != last; ++first){
        thread_pool.do_task(fun, first);
    }

    thread_pool.wait();
}

template<typename TP, typename Container, typename Functor>
void parallel_foreach_it(TP& thread_pool, Container& container, Functor fun){
    using std::begin;
    using std::end;

    parallel_foreach_it(thread_pool, begin(container), end(container), fun);
}

// Parallel for_each giving the index to the functor

template<typename TP, typename Iterator, typename Functor>
void parallel_foreach_i_only(TP& thread_pool, Iterator first, Iterator last, Functor fun){
    if(std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value){
        parallel_foreach_n(thread_pool, 0, std::distance(first, last), fun);
    } else {
        for(std::size_t i = 0; first != last; ++first, ++i){
            thread_pool.do_task(fun, i);
        }

        thread_pool.wait();
    }
}

template<typename TP, typename Container, typename Functor>
void parallel_foreach_i_only(TP& thread_pool, Container& container, Functor fun){
    parallel_foreach_n(thread_pool, 0, container.size(), fun);
}

// Parallel for_each on a range

template<typename TP, typename Functor>
void parallel_foreach_n(TP& thread_pool, std::size_t first, std::size_t last, Functor fun){
    auto n = last - first;
    auto part = n / thread_pool.size();

    if(part < 2){
        for(std::size_t i = first; i < last; ++i){
            thread_pool.do_task(fun, i);
        }
    } else {
        auto batch_functor = [fun](std::size_t first, std::size_t last){
            for(std::size_t i = first; i < last; ++i){
                fun(i);
            }
        };

        //Distribute evenly the batches

        for(std::size_t i = first; i + part < last; i += part){
            thread_pool.do_task(batch_functor, i, i + part);
        }

        //Distribute the remainders

        auto rem = n % thread_pool.size();
        if(rem > 0){
            for(std::size_t i = last - rem; i < last; ++i){
                thread_pool.do_task(fun, i);
            }
        }
    }

    thread_pool.wait();
}

//Special version with pair

template<typename TP, typename Iterator, typename Iterator2, typename Functor>
void parallel_foreach_pair_i(TP& thread_pool, Iterator f_first, Iterator f_last, Iterator2 s_first, Iterator2 s_last, Functor fun){
    cpp_unused(s_last);

    for(std::size_t i = 0; f_first != f_last; ++f_first, ++s_first, ++i){
        thread_pool.do_task(fun, *f_first, *s_first, i);
    }

    thread_pool.wait();
}

template<template<typename...> class queue_t = std::deque>
struct default_thread_pool {
    enum class thread_status {
        WAITING,
        WORKING
    };

private:
    std::vector<std::thread> threads;
    std::vector<thread_status> status;
    queue_t<std::function<void()>> tasks;
    std::mutex main_lock;
    std::condition_variable condition;
    std::condition_variable wait_condition;
    volatile bool stop_flag = false;

public:
    default_thread_pool(std::size_t n) : status(n, thread_status::WAITING) {
        for(std::size_t t = 0; t < n; ++t){
            threads.emplace_back([this, t]{
                while(true){
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> ulock(main_lock);

                        status[t] = thread_status::WAITING;

                        wait_condition.notify_one();

                        condition.wait(ulock, [this]{ return stop_flag || !tasks.empty(); });

                        if(stop_flag && tasks.empty()){
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop_front();

                        status[t] = thread_status::WORKING;
                    }

                    task();
                }
            });
        }
    }

    default_thread_pool() : default_thread_pool(std::thread::hardware_concurrency()) {}

    ~default_thread_pool(){
        with_lock(main_lock, [this]{ stop_flag = true; });

        condition.notify_all();

        for(auto& thread : threads){
            thread.join();
        }
    }

    std::size_t size() const {
        return threads.size();
    }

    void wait(){
        while(true){
            std::unique_lock<std::mutex> ulock(main_lock);

            if(tasks.empty() && std::find(status.begin(), status.end(), thread_status::WORKING) == status.end()){
                return;
            }

            //At this point, there are still some threads working, we wait for
            //one of them to notify a change of status
            wait_condition.wait(ulock, [this]{ return tasks.empty(); });
        }
    }

    template<class Functor, typename... Args>
    void do_task(Functor fun, Args... args){
        with_lock(main_lock, [fun, args..., this](){
            if(stop_flag){
#ifndef CPP_UTILS_NO_EXCEPT
                throw std::runtime_error("cpp_utils: enqueue on stopped ThreadPool");
#else
                std::cerr << "cpp_utils: enqueue on stopped ThreadPool (exceptions disabled)" << std::endl;
                std::abort();
#endif
            }

            tasks.emplace_back([fun, args...]{ fun(args...); });
        });

        condition.notify_one();
    }
};

} //end of the cpp namespace

#endif //CPP_UTILS_PARALLEL_HPP

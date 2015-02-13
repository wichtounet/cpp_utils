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

namespace cpp {

//Utilities

template<typename Lock, typename Functor>
void with_lock(Lock& lock, Functor&& fun){
    std::unique_lock<Lock> l(lock);
    fun();
}

//Normal parallel_foreach versions

template<typename Iterator, typename Functor>
void parallel_foreach(Iterator first, Iterator last, Functor&& fun){
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for(; first != last; ++first){
        futures.push_back(std::move(std::async(std::launch::async, fun, *first)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Iterator, typename Functor>
void parallel_foreach_i(Iterator first, Iterator last, Functor&& fun){
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for(; first != last; ++first){
        futures.push_back(std::move(std::async(std::launch::async, fun, *first, futures.size())));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Container, typename Functor>
void parallel_foreach_i(const Container& container, Functor&& fun){
    std::vector<std::future<void>> futures;
    futures.reserve(container.size());

    for(std::size_t i = 0; i < container.size(); ++i){
        futures.push_back(std::move(std::async(std::launch::async, fun, container[i], i)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

//parallel_foreach Thread pool versions

template<typename TP, typename Iterator, typename Functor>
void parallel_foreach(TP& thread_pool, Iterator first, Iterator last, Functor&& fun){
    for(; first != last; ++first){
        thread_pool.do_task(std::forward<Functor>(fun), *first);
    }

    thread_pool.wait();
}

template<typename TP, typename Iterator, typename Functor>
void parallel_foreach_i(TP& thread_pool, Iterator first, Iterator last, Functor&& fun){
    for(std::size_t i = 0; first != last; ++first, ++i){
        thread_pool.do_task(std::forward<Functor>(fun), *first, i);
    }

    thread_pool.wait();
}

template<typename TP, typename Iterator, typename Iterator2, typename Functor>
void parallel_foreach_pair_i(TP& thread_pool, Iterator f_first, Iterator f_last, Iterator2 s_first, Iterator2 s_last, Functor&& fun){
    cpp_unused(s_last);

    for(std::size_t i = 0; f_first != f_last; ++f_first, ++s_first, ++i){
        thread_pool.do_task(std::forward<Functor>(fun), *f_first, *s_first, i);
    }

    thread_pool.wait();
}

template<typename TP, typename Container, typename Functor>
void parallel_foreach_i(TP& thread_pool, const Container& container, Functor&& fun){
    for(std::size_t i = 0; i < container.size(); ++i){
        thread_pool.do_task(std::forward<Functor>(fun), container[i], i);
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
    void do_task(Functor fun, Args&&... args){
        with_lock(main_lock, [fun, &args..., this](){
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

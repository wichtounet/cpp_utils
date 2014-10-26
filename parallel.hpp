//=======================================================================
// Copyright (c) 2013-2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef CPP_UTILS_PARALLEL_HPP
#define CPP_UTILS_PARALLEL_HPP

#include<thread>
#include<future>
#include<vector>
#include<deque>
#include<functional>

namespace cpp {

template<typename Iterator, typename Functor>
void parallel_foreach(Iterator first, Iterator last, Functor&& fun){
    std::vector<std::future<void>> futures;
    futures.reserve(std::distance(first, last));

    for(; first != last; ++first){
        futures.push_back(std::move(std::async(std::launch::async, fun, *first)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Container, typename Functor>
void parallel_foreach_i(const Container& container, Functor&& fun){
    std::vector<std::future<void>> futures;
    futures.reserve(container.size());

    for(std::size_t i = 0; i < container.size(); ++i){
        futures.push_back(std::move(std::async(std::launch::async, fun, i)));
    }

    //No need to wait for the futures, the destructor will do it for us
}

template<typename Lock, typename Functor>
void with_lock(Lock& lock, Functor&& fun){
    std::unique_lock<Lock> l(lock);
    fun();
}

enum class thread_status {
    WAITING,
    WORKING
};

template<template<typename...> class queue_t = std::deque>
struct default_thread_pool {
private:
    std::vector<std::thread> threads;
    std::vector<thread_status> status;
    queue_t<std::function<void()>> tasks;
    std::mutex main_lock;
    std::condition_variable condition;
    volatile bool stop_flag = false;

public:
    default_thread_pool(std::size_t n) : status(n, thread_status::WAITING) {
        for(std::size_t t = 0; t < n; ++t){
            threads.emplace_back(
                [this, t]
                {
                    while(true){
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> ulock(main_lock);

                            status[t] = thread_status::WAITING;

                            condition.wait(ulock, [this]{return stop_flag || !tasks.empty(); });

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
        {
            std::unique_lock<std::mutex> ulock(main_lock);
            stop_flag = true;
        }

        condition.notify_all();

        for(auto& thread : threads){
            thread.join();
        }
    }

    //TODO DO better than busy waiting
    void wait(){
        while(true){
            std::unique_lock<std::mutex> ulock(main_lock);

            if(tasks.empty()){
                bool still_working = false;
                for(auto s : status){
                    if(s == thread_status::WORKING){
                        still_working = true;
                        break;
                    }
                }

                if(!still_working){
                    return;
                }
            }
        }
    }

    template<class Functor, typename... Args>
    void do_task(Functor&& fun, Args&&... args){
        {
            std::unique_lock<std::mutex> ulock(main_lock);

            if(stop_flag){
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }

            tasks.emplace_back([&fun, args...](){ fun(args...); });
        }

        condition.notify_one();
    }

};

} //end of the cpp namespace

#endif //CPP_UTILS_PARALLEL_HPP

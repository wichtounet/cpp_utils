//=======================================================================
// Copyright (c) 2013-2020 Baptiste Wicht
// Distributed under the terms of the MIT License.  // (See accompanying file LICENSE or copy at //  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains thread pool implementations.
 */

#ifndef CPP_UTILS_THREAD_POOL_HPP
#define CPP_UTILS_THREAD_POOL_HPP

#ifdef CPP_UTILS_NO_EXCEPT
#include <iostream>
#endif

#include <thread>
#include <future>
#include <vector>
#include <deque>
#include <functional>
#include <algorithm>

#include "assert.hpp"
#include "tmp.hpp"

namespace cpp {

/*!
 * \brief The default thread pool of hte library.
 *
 * The pool is created with a certain number of threads and this number of
 * threads cannot change. The jobs are put into a queue and threads are
 * notified. Threads are constantly waiting (passively) for jobs to be enqueue.
 *
 * \tparam queue_t The type of queue to use (std::deque by default)
 */
template <template <typename...> class queue_t = std::deque>
struct default_thread_pool {
    /*!
     * \brief Enumeration for the status of threads
     */
    enum class thread_status {
        WAITING, ///< The thread is waiting on a task
        WORKING  ///< The thread is working on a task
    };

private:
    std::vector<std::thread> threads;                                            ///< The current threads
    std::vector<thread_status> status;                                           ///< The status of each thread
    queue_t<std::function<void()>, std::allocator<std::function<void()>>> tasks; ///< The queue of tasks
    std::mutex main_lock;                                                        ///< The main lock mutex
    std::condition_variable condition;                                           ///< The global condition variable for the main thread
    std::condition_variable wait_condition;                                      ///< The condition variable for the threads waiting for tasks
    volatile bool stop_flag = false;                                             ///< Flag indicating if the thread pool is currently being release

public:
    /*!
     * \brief Construct a thread pool with the given number of threads
     * \param n The number of threads
     */
    explicit default_thread_pool(std::size_t n) : status(n, thread_status::WAITING) {
        // The thread pool is of fixed size, avoid any possible reallocation
        threads.reserve(n);

        // Lets all the threads start at the same time and avoid any possible race conditions
        std::unique_lock<std::mutex> ulock(main_lock);

        for (std::size_t t = 0; t < n; ++t) {
            threads.emplace_back([this, t] {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> ulock(main_lock);

                        status[t] = thread_status::WAITING;

                        wait_condition.notify_one();

                        condition.wait(ulock, [this] {
                            return stop_flag || !tasks.empty();
                        });

                        if (stop_flag && tasks.empty()) {
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

    /*!
     * \brief Construct a thread pool with as many threads as the hardware has concurrency
     */
    default_thread_pool()
            : default_thread_pool(std::thread::hardware_concurrency()) {}

    /*!
     * \brief Destroys the thread pool and wait for the threads to be done
     */
    ~default_thread_pool() {
        with_lock(main_lock, [this] { stop_flag = true; });

        condition.notify_all();

        for (auto& thread : threads) {
            thread.join();
        }
    }

    /*!
     * \brief return the number of threads of the thread pool
     * \return the number of threads of the thread pool
     */
    std::size_t size() const {
        return threads.size();
    }

    /*!
     * \brief Wait for every thread to be done
     */
    void wait() {
        while (true) {
            std::unique_lock<std::mutex> ulock(main_lock);

            if (tasks.empty() && std::find(status.begin(), status.end(), thread_status::WORKING) == status.end()) {
                return;
            }

            //At this point, there are still some threads working, we wait for
            //one of them to notify a change of status
            wait_condition.wait(ulock, [this] {
                return tasks.empty();
            });
        }
    }

    /*!
     * \brief Submit a task to the pool
     * \param fun The functor to execute
     * \param args The arguments to be passed to the functor
     */
    template <class Functor, typename... Args>
    void do_task(Functor fun, Args... args) {
        with_lock(main_lock, [fun, args..., this] () mutable {
            if (stop_flag) {
#ifndef CPP_UTILS_NO_EXCEPT
                throw std::runtime_error("cpp_utils: enqueue on stopped ThreadPool");
#else
                std::cerr << "cpp_utils: enqueue on stopped ThreadPool (exceptions disabled)" << std::endl;
                std::abort();
#endif
            }

            //Execute the task
            tasks.emplace_back([fun, args...] () mutable {
                fun(args...);
            });
        });

        condition.notify_one();
    }
};

} //end of the cpp namespace

#endif //CPP_UTILS_THREAD_POOL_HPP

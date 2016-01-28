//=======================================================================
// Copyright Baptiste Wicht 2013-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file stop_watch.hpp
 * \brief Contains stop watches implementation to measure duration
 */

#ifndef CPP_STOP_WATCH_HPP
#define CPP_STOP_WATCH_HPP

#include <chrono>
#include <iostream>

namespace cpp {

/*!
 * \brief Default clock type used by the library
 */
using clock_type = std::chrono::high_resolution_clock;

/*!
 * \class stop_watch
 * \brief A stop watch
 * \tparam P The std::chrono precision used by the watch.
 *
 * The watch automatically starts when the constructor is called
 */
template <typename P = std::chrono::milliseconds>
class stop_watch {
public:
    /*!
     * \brief The std::chrono precision used by the watch.
     */
    using precision = P;

    /*!
     * \brief Constructs a new stop_watch and starts it.
     */
    stop_watch() {
        start_point = clock_type::now();
    }

    /*!
     * \brief Return the elapsed time since construction.
     * \return the elapsed time since construction.
     */
    double elapsed() {
        auto end_point = clock_type::now();
        auto time = std::chrono::duration_cast<precision>(end_point - start_point);
        return time.count();
    }

private:
    clock_type::time_point start_point;
};

/*!
 * \class auto_stop_watch
 * \brief An automatic stop watch
 * \tparam P The std::chrono precision used by the watch.
 *
 * The watch automatically starts when the constructor is called and display the duration when destructed.
 */
template <typename P = std::chrono::milliseconds>
class auto_stop_watch {
public:
    /*!
     * \brief The std::chrono precision used by the watch.
     */
    using precision = P;

    /*!
     * \brief Constructs a new auto_stop_watch and starts it.
     * \param title The title that will be displayed when the watch is over.
     */
    explicit auto_stop_watch(std::string title)
            : title(std::move(title)) {
        //Empty
    }

    /*!
     * \brief Destroys the auto_stop_watch and display the elapsed time.
     */
    ~auto_stop_watch() {
        std::cout << title << " took " << watch.elapsed() << std::endl;
    }

private:
    std::string title;
    stop_watch<precision> watch;
};

} //end of cpp namespace

#endif //CPP_STOP_WATCH_HPP

//
// Created by Andy Mina on 11/3/23.
//

#ifndef SEAM_CARVING_CLOCK_HPP
#define SEAM_CARVING_CLOCK_HPP

#include <chrono>

template <class Duration = std::chrono::milliseconds, class Clock = std::chrono::steady_clock>
class Timer {
    using time_pt = typename Clock::time_point;

    time_pt _start = Clock::now();
    time_pt _end = {};

public:
    void tick() { _start = Clock::now(); }
    void tock() { _end = Clock::now(); }

    template <class T = Duration>
    auto duration() const {
        return std::chrono::duration_cast<T>(_end - _start).count();
    }
};

#endif //SEAM_CARVING_CLOCK_HPP

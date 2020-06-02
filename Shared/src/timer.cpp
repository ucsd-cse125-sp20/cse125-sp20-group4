#include "timer.h"

Timer::Timer(time_t start, double duration, std::function<void()> callback) : start(start), duration(duration), remaining(duration), callback(callback) {}

Timer::Timer(const Timer& timer) {
    start = timer.start;
    duration = timer.duration;
    remaining = timer.remaining;
    callback = timer.callback;
}

double Timer::getRemaining() {
    return remaining;
}

std::function<void()> Timer::update(time_t now) {
    remaining = duration - difftime(now, start) * 1000.0;
    if (remaining <= 0.0) {
        return callback;
    }
    return NULL;
}
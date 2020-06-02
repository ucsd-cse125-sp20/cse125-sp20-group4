#pragma once
#include <ctime>
#include <functional>

#define NO_CALLBACK [](){}

class Timer {
private:
    time_t start;
    // all time in ms
    double duration;
    double remaining;
    std::function<void()> callback;

public:
    Timer(time_t, double, std::function<void()>);
    Timer(const Timer&);

    double getRemaining();
    // returns callback
    std::function<void()> update(time_t);
};
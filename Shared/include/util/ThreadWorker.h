#ifndef THREAD_WORKER_H
#define THREAD_WORKER_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

/**
 * A worker that repeatedly executes a certain action indefinitely in a separate thread.
 *
 * If an unhandled exception occurs at any iteration, the worker will stop.
 */
class ThreadWorker {

    public:
    /**
     * Creates a new worker that executes the given action.
     *
     * @param act The action to execute.
     * @throws std::invalid_argument if act is empty.
     */
    ThreadWorker( const std::function<bool()> act );
    /**
     * Stops the worker. Will block until it is fully stopped.
     */
    ~ThreadWorker();

    /**
     * Starts execution.
     */
    void start();
    /**
     * Signals to stop execution. The worker will stop on the next iteration.
     */
    void stop();
    /**
     * Blocks until the worker starts execution.
     */
    void waitReady();
    /**
     * Blocks until the worker terminates.
     */
    void waitDone();

    protected:
    /**
     * Creates a new worker that executes the given action, if any.
     *
     * If the given action is empty, action() should be overriden, otherwise it will throw
     * and exeption and the worker will die.
     *
     * @param act The action to execute.
     * @param allowEmptyAct If false, does not allow an emtpy act to be given.
     * @throws std::invalid_argument if allowEmptyAct is false and act is empty.
     */
    ThreadWorker( const std::function<bool()> act, bool allowEmptyAct );

    /**
     * Executes one work action.
     *
     * By default, calls the action defined by act.
     *
     * @return If false, signals that the worker should stop.
     */
    virtual bool action();

    /* (by default) the action to execute at each iteration. */
    const std::function<bool()> act;

    private:
    std::thread worker;
    std::atomic<bool> running;
    bool ready;
    std::mutex readyMutex;
    std::condition_variable readyCond;
    std::mutex joinMutex;

    /**
     * Thread driver.
     */
    void run();

};

#endif
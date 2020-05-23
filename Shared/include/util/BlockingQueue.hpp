#ifndef BLOCKING_QUEUE
#define BLOCKING_QUEUE

#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>

/**
 * A thread-safe queue that blocks on pop() until an element is available.
 *
 * @tparam T Type of object stored in the queue.
 * @tparam T Allocator for new objects.
 */
template <class T, class Alloc = allocator<T>> 
class BlockingQueue {

    public:
    /**
     * Instantiates a new queue.
     */
    BlockingQueue() {}

    using std::deque::size_type;

    /**
     * Pushes a new element into the queue.
     *
     * @param e The element to push.
     */
    void push( const T & e ) {

        std::unique_lock<std::mutex> lck( mtx );
        queue.push_back( e );
        cv.notify_one();

    }

    /**
     * Pops the next element from the queue. If there are no elements available,
     * will block until there is one.
     *
     * A timeout may be provided to limit how long the call may block while waiting
     * for an element to become available. Note that, technically, it is possible for
     * a timeout to occur even if there are elements in the queue if the time it takes
     * to obtain the queue's internal lock exceeds the timeout. However, this is very
     * unlikely to happen for a reasonable timeout.
     *
     * @param dest Where to place the popped element.
     * @param timeout The maximum amount of time to block while waiting for an element
     *                to become available, in milliseconds. If 0, waits indefinitely.
     * @return True if an element was successfully popped, false if a timeout occurred
     *         before an element became available.
     */
    bool pop( T & dest, const unsigned long timeout = 0 ) {

        const std::chrono::time_point maxTime = std::chrono::steady_clock::now() + std::chrono::milliseconds( timeout );
        std::unique_lock<std::mutex> lck( mtx, maxTime );
        if ( !lck ) {
            return false; // Timed out before obtaining lock
        }

        // Wait until there is an element to return
        if ( timeout == 0L ) {
            cv.wait( lck, this->hasElement );
        } else {
            if ( !cv.wait_until( lck, maxTime, this->hasElement ) ) {
                return false; // Timed out
            }
        }

        // Pop value and return
        dest = queue.front();
        queue.pop_front();
        return true;

    }

    /**
     * Pops the next element from the queue, if one is available.
     *
     * @param dest Where to place the popped element.
     * @return True if an element was successfully popped, false if there are no
     *         elements available.
     */
    bool tryPop( T & dest ) {

        std::unique_lock<std::mutex> lck( mtx );
        if ( hasElement() ) {
            dest = queue.front();
            queue.pop_front();
            return true;
        } else {
            return false;
        }

    }

    /**
     * Pops all elements currently in the queue.
     *
     * @param dest The queue to place the popped events into.
     *             Elements are placed so that the oldest element is at
     *             the front of the queue.
     */
    void popAll( std::deque<T> & dest ) {

        std::unique_lock<std::mutex> lck( mtx );
        while ( hasElement() ) {
            dest.push_back( queue.front() );
            queue.pop_front();
        }

    }

    /**
     * Retrieves the current amount of elements in the queue. 
     * 
     * Note that, while this is thread-safe, it only represents the size of the queue 
     * in the instant that the function executed, and the size may have been modified 
     * by other threads at any point after it returned.
     *
     * @return The size of the queue.
     */
    size_type size() const {

        std::unique_lock<std::mutex> lck( mtx );
        return queue.size();

    }

    protected:
    /* Mutex that synchronizes the queue */
    std::mutex mtx;
    /* Condition Variable popping threads wait on */
    std::condition_variable cv;
    /* Internal storage queue */
    std::deque<T, Alloc> queue;

    /**
     * Determines if there is an element in the queue.
     *
     * This method is NOT thread-safe by itself.
     *
     * @return True if there is an element in the queue, false otherwise.
     */
    bool hasElement() const {

        return !queue.empty();

    }

};

#endif
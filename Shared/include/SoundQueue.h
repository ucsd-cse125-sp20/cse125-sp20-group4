#ifndef SOUND_QUEUE_H
#define SOUND_QUEUE_H

#include "EventClasses/SoundEvent.h"
#include "util/BlockingQueue.hpp"

class SoundQueue {

    public:
    static void push( const std::shared_ptr<SoundEvent> & e );
    static void popAll( std::deque<std::shared_ptr<SoundEvent>> & dest );
    
    private:
    static BlockingQueue<std::shared_ptr<SoundEvent>> q;

};

#endif
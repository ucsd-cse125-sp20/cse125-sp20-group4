#include "SoundQueue.h"

BlockingQueue<std::shared_ptr<SoundEvent>> SoundQueue::q;

void SoundQueue::push( const std::shared_ptr<SoundEvent> & e ) {

    q.push( e );

}

void SoundQueue::popAll( std::deque<std::shared_ptr<SoundEvent>> & dest ) {

    q.popAll( dest );

}
#include "../include/singleton.h"

#include <queue>

static std::queue< std::function<void()> > finalizer_queue;

void singleton_finalizer::add_finalizer( std::function<void()> func ){
    finalizer_queue.push( func );
}

void singleton_finalizer::finalize() {
    while( !finalizer_queue.empty() ){
        finalizer_queue.front()();
        finalizer_queue.pop();
    }
}
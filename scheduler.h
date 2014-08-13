//
//  scheduler.h
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/12/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#ifndef TPSO1_scheduler_h
#define TPSO1_scheduler_h

#include <queue>
#include "thread.h"

class Scheduler {
private:
    // thread_queue is a heap. The idea is to always execute the thread with the highest
    // priority, decrement its priority and reinsert it into the heap. When the priority
    // reaches 0, it is reset to the initial priority.
    std::priority_queue<Thread, std::deque, Thread_Greater> m_thread_queue;
    
public:
    Scheduler();
    void push (Thread);
    Thread pop ();
    
};







#endif

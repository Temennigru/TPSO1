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

class __Internal_Scheduler__ {
private:
    // thread_queue is a heap. The idea is to always execute the thread with the highest
    // priority, decrement its priority and reinsert it into the heap. When the priority
    // reaches 0, it is reset to the initial priority.
    std::vector<Thread*> m_zero_priority;
    std::priority_queue<Thread*, std::deque<Thread*>, Thread_Greater> m_thread_queue;
    ThreadPtr current_thread;
    
    void push (Thread*);
    Thread* pop();
    void HandleAlarm(int signal);
    __Internal_Scheduler__();

    
public:
    void SchedulerMain();
    void new_thread(void (*func)(int));
    ucontext_t main_context;
    
    friend class Scheduler;
    
};


class Scheduler {
public:
    __Internal_Scheduler__* main_scheduler;
    Scheduler();
};

extern __Internal_Scheduler__* __main_scheduler__; // Singleton scheduler





#endif

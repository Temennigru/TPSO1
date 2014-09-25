//
//  scheduler.h
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/12/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#ifndef TPSO1_scheduler_h
#define TPSO1_scheduler_h

#include <sys/time.h>
#include <queue>
#include <set>
#include <map>
#include "thread.h"

class __Internal_Scheduler__ {
private:
    // thread_queue is a heap. The idea is to always execute the thread with the highest
    // priority, decrement its priority and reinsert it into the heap. When the priority
    // reaches 0, it is reset to the initial priority.
    std::vector<Thread*> m_zero_priority;
    std::priority_queue<Thread*, std::deque<Thread*>, Thread_Greater> m_thread_queue;
    ThreadPtr m_current_thread;
    struct itimerval m_thread_timer;
    struct itimerval m_sleep_timer;
    
    void push (Thread*);
    Thread* pop();
    static void HandleAlarm(int signal);
    __Internal_Scheduler__() {}
    __Internal_Scheduler__(void (*callback)(), int param);
    std::set<int> ids;
    std::vector<ThreadPtr> sleeping;
    std::vector<std::pair<int, ThreadPtr> > waiting;

    
public:
    void SchedulerMain();
    int* NewThread(void (*func)(), int param);
    ucontext_t main_context;

    void ThreadYield();
    void ThreadSleep(struct timespec);
    void ThreadExit();
    void ThreadWait(int threadId);
    
    friend class Scheduler;
    
};


class Scheduler {
public:
    __Internal_Scheduler__* main_scheduler;
    Scheduler(void (*callback)(), int param);
    Scheduler();
};

extern __Internal_Scheduler__* __main_scheduler__; // Singleton scheduler





#endif

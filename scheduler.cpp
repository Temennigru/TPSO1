//
//  scheduler.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/13/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "scheduler.h"
#include "util.h"
#include <assert.h>
#include <sys/time.h>
#include <signal.h>


__Internal_Scheduler__* __main_scheduler__ = NULL;
ucontext_t* __main_context__;

Scheduler::Scheduler(void (*callback)(), int param) {
    if (__main_scheduler__ == NULL) {
        __main_scheduler__ = new __Internal_Scheduler__(callback, param);
    }
    this->main_scheduler = __main_scheduler__;
}

Scheduler() {
    if (__main_scheduler__ == NULL) {
        abort();
    }
    this->main_scheduler = __main_scheduler__;
}

__Internal_Scheduler__::__Internal_Scheduler__(void (*callback)(), int param) {
    __main_context__ = &this->main_context;

    this->main_context.uc_link = NULL;
    this->main_context.uc_stack.ss_sp = malloc (STANDARD_STACK_SIZE);
    this->main_context.uc_stack.ss_size = STANDARD_STACK_SIZE;

    this->m_current_thread = NULL;

    this->NewThread(callback, param);

    alarm(ITIMER_REAL, 10000, this->m_sleep_timer, true); // Same resolution as unix alarm.

    getcontext(&this->main_context);

    this->SchedulerMain();
}

void __Internal_Scheduler__::HandleAlarm(int signal) {
    if (signal == SIGVTALRM) { // Thread alarm
        swapcontext(&__main_scheduler__->m_current_thread->m_context, &__main_scheduler__->main_context);
    } else if (signal == SIGALRM) { // Sleep alarm
        for (std::vector<ThreadPtr>::const_iterator it = __main_scheduler__->sleeping.begin();
             it != __main_scheduler__->sleeping.end(); it++) {

            (*it)->m_sleep--;
            if ((*it)->m_sleep == 0) {
                __main_scheduler__->push(*it);
            }
        }
    }
}

void __Internal_Scheduler__::push (Thread* t) {
    
    // Processes with 0 priority do not run until all
    // processes have finished running
    if (t->m_priority == 0) {
        t->m_priority = t->m_init_priority;
        this->m_zero_priority.push_back(t);
    }
    else {
        t->m_priority--;
        this->m_thread_queue.push(t);
    }
    
}

Thread* __Internal_Scheduler__::pop () {

    assert( !this->m_thread_queue.empty() && !this->m_zero_priority.empty() );
    
    // All processes have finished running. Re-insert zero-priority
    // processes into queue with max priority
    if ( this->m_thread_queue.empty() ) {
        while ( !this->m_zero_priority.empty() ) {
            this->m_thread_queue.push(this->m_zero_priority.back());
            this->m_zero_priority.pop_back();
        }
    }
    
    Thread* t = this->m_thread_queue.top();
    this->m_thread_queue.pop();
    
    return t;
    
}

void __Internal_Scheduler__::SchedulerMain() {

    signal (SIGVTALRM, __Internal_Scheduler__::HandleAlarm);

    signal (SIGALRM, __Internal_Scheduler__::HandleAlarm);

    if (this->m_current_thread == NULL) {
        this->m_current_thread = this->pop();
        alarm(ITIMER_VIRTUAL, RUNTIME, this->m_thread_timer, false);
        this->m_current_thread->restore();
    } else {
        if (this->m_current_thread->m_death) {
            for (std::vector<std::pair<int, ThreadPtr> >::const_iterator it = this->waiting.begin();
                 it != this->waiting.end(); it++) {
                if (it->first == this->m_current_thread->m_id) {
                    this->push(it->second);
                    this->waiting.erase(it);
                }
            }
            delete this->m_current_thread;
        } else {
            this->push(this->m_current_thread);
        }
        this->m_current_thread = this->pop();
        alarm(ITIMER_VIRTUAL, RUNTIME, this->m_thread_timer, false);
        this->m_current_thread->restore();
    }
    
    
}

int* __Internal_Scheduler__::NewThread(void (*func)(), int param) {
    int i = 0;
    // Find first available thread Id. If there are more than 4294967295 threads running,
    // this will error out (impossible unless the system has more than 2 petabytes of stack space).
    for (i = 0; ; i++) {
        if (this->ids.find(i) != this->ids.end()) break;
    }
    ThreadPtr newThread = new Thread (func, param, i);

    this->push(newThread);

    return &newThread->m_id;
}

void __Internal_Scheduler__::ThreadYield() {
    alarmDisable(this->m_thread_timer);
    swapcontext(&this->m_current_thread->m_context, &this->main_context);
}

void __Internal_Scheduler__::ThreadSleep(struct timespec time) {
    this->m_current_thread->m_sleep = (time.tv_sec * 100) + (time.tv_nsec / 10000000); // Convert to 1 centisecond resolution
    if (time.tv_nsec > 0 && this->m_current_thread->m_sleep == 0) {
        this->m_current_thread->m_sleep = 1;
    }
    this->sleeping.push_back(this->m_current_thread);
    ThreadPtr temp = this->m_current_thread;
    this->m_current_thread = NULL;
    swapcontext(&temp->m_context, &this->main_context);
}

void __Internal_Scheduler__::ThreadExit() {
    this->m_current_thread->MarkForDeath();
    this->ThreadYield();
}

void __Internal_Scheduler__::ThreadWait(int threadId) {
    this->waiting.push_back(std::make_pair(threadId, this->m_current_thread));
    ThreadPtr temp = this->m_current_thread;
    this->m_current_thread = NULL;
    swapcontext(&temp->m_context, &this->main_context);
}
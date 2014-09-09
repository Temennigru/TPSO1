//
//  scheduler.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/13/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "scheduler.h"
#include <assert.h>
#include <sys/time.h>

__Internal_Scheduler__* __main_scheduler__ = NULL;
ucontext_t* __main_context__ = NULL;

Scheduler::Scheduler() {
    if (__main_scheduler__ == NULL) {
        __main_scheduler__ = new __Internal_Scheduler__();
    }
    this->main_scheduler = __main_scheduler__;
}

__Internal_Scheduler__::__Internal_Scheduler__() {
    __main_context__ = &(this->main_context);
}

void __Internal_Scheduler__::HandleAlarm(int signal) {
    swapcontext(&this->current_thread->m_context, &this->main_context);
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
    
    struct itimerval it;
    /*
    .uc_link = &uc[0];
    .uc_stack.ss_sp = st1;
    .uc_stack.ss_size = sizeof st1;
    makecontext (&uc[1], (void (*) (void)) f, 1, 1);
    */
    
    
}

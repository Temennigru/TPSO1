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


__Internal_Scheduler__* __main_scheduler__ = NULL;
ucontext_t* __main_context__;

Scheduler::Scheduler() {
    if (__main_scheduler__ == NULL) {
        __main_scheduler__ = new __Internal_Scheduler__();
    }
    this->main_scheduler = __main_scheduler__;
}

__Internal_Scheduler__::__Internal_Scheduler__(void (*callback)(), int param) {
    __main_context__ = &this->main_context;

    this->main_context.uc_link = NULL;
    this->main_context.uc_stack.ss_sp = malloc (STANDARD_STACK_SIZE);
    this->main_context.uc_stack.ss_size = STANDARD_STACK_SIZE;

    this->current_thread = NULL;

    this->NewThread(callback, param);
    
    getcontext(&this->main_context);

    this->SchedulerMain();
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

void __Internal_Scheduler__::NewThread(void (*func)(), int param) {
    int i = 0;
    // Find first available thread Id. If there are more than 4294967295 threads running,
    // this will error out (impossible unless the system has more than 2 petabytes of stack space).
    for (i = 0; ; i++) {
        if (this->ids.find(i) != this->ids.end()) break;
    }
    ThreadPtr newThread = new Thread (func, param, i);

    this->push(newThread);
}

void __Internal_Scheduler__::SchedulerMain() {

    if (this->current_thread == NULL) {
        this->current_thread = this->pop();
        alarm(RUNTIME);
        swapcontext(&current_thread->m_context, &this->main_context);
    } else {
        this->push(this->current_thread);
        this->current_thread = this->pop();
        alarm(RUNTIME);
        setcontext(&current_thread->m_context);
    }
    
    
}

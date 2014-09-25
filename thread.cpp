//
//  thread.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/20/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "thread.h"
#include "util.h"
#include <cstdlib>
#include <ucontext.h>

Thread::Thread(void(*callback)(), int param, int threadId, int priority) : main_context(*__main_context__) {

    this->m_callback = callback;
    this->m_param = param;
    this->m_init_priority = priority;
    this->m_priority = priority;
    this->stack = (Stack) malloc (STANDARD_STACK_SIZE);
    this->m_id = threadId;


    this->m_context.uc_link = __main_context__;
    this->m_context.uc_stack.ss_sp = this->stack;
    this->m_context.uc_stack.ss_size = STANDARD_STACK_SIZE;
    getcontext(&this->m_context);
    makecontext(&this->m_context, this->m_callback(), 1, this->m_param);
}

Thread::~Thread() {
    free(this->stack);
}

void MarkForDeath() { this->m_death = true; }

void Thread::save() { getcontext(&this->m_context); }
void Thread::restore() { setcontext(&this->m_context); }
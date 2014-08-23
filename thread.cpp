//
//  thread.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/20/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "thread.h"
#include <cstdlib>

Thread::Thread(void(*callback)(int), int param, int priority) {

    this->m_callback = callback;
    this->m_param = param;
    this->m_init_priority = priority;
    this->m_priority = priority;
    this->stack = (Stack) malloc (2097152);
    
}

/* TODO:
 * Fix circular dependency in Thread::exec
 */

void Thread::exec() {
    signal (SIGALRM, __main_scheduler__::HandleAlarm);
    swapcontext(__main_scheduler__::main_context, this->m_context);
}
void Thread::save() { getcontext(&this->m_context); }
void Thread::restore() { setcontext(&this->m_context); }
//
//  thread.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/20/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "thread.h"
#include <cstdlib>

Thread::Thread(void(*callback)(int), int param, int priority) : main_context(*__main_context__) {

    this->m_callback = callback;
    this->m_param = param;
    this->m_init_priority = priority;
    this->m_priority = priority;
    this->stack = (Stack) malloc (2097152);
    
}

/* TODO:
 * Make scheduler set alarm before running exec.
 * Fix restore() not saving current context.
 */

void Thread::exec() {
    swapcontext(&this->main_context, &this->m_context);
}
void Thread::save() { getcontext(&this->m_context); }
void Thread::restore() { setcontext(&this->m_context); }
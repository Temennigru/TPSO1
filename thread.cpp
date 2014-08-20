//
//  thread.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/20/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "thread.h"


Thread::Thread(void(*callback)(int), int param, int priority) {

    this->m_callback = callback;
    this->m_param = param;
    this->m_init_priority = priority;
    this->m_priority = priority;
    
}

void Thread::exec() { this->m_callback(this->m_param); }
void Thread::save() { getcontext(&this->m_context); }
void Thread::restore() { setcontext(&this->m_context); }
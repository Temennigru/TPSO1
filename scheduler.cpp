//
//  scheduler.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/13/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "scheduler.h"



void Scheduler::push (Thread& t) {
    
    if (t.m_priority == 0) { t.m_priority = t.m_init_priority; }
    else { t.m_priority--; }
    
    
    this->m_thread_queue.push_back(t);
    
}

Thread& Scheduler::pop () {

    assert(!this->m_thread_queue.empty() );
    
    
    Thread& t = this->m_thread_queue.front();
    this->m_thread_queue.pop();
    
    return t;
    
}


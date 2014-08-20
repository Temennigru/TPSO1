//
//  thread.h
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/12/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#ifndef TPSO1_thread_h
#define TPSO1_thread_h

#include <ucontext.h>

struct Thread_Greater;

class Thread {
private:
    ucontext_t m_context;
    void(*m_callback)();
    int m_param;
    int m_init_priority;
    int m_priority;
    
public:
    Thread(void(*callback)(), int param, int priority = 0);
    void exec();
    void save();
    void restore();
    void reset_priority();
    
    
    
    friend struct Thread_Greater;
    friend class Scheduler;
};


struct Thread_Greater : std::binary_function <Thread,Thread,bool> {
    bool operator() (const Thread& x, const Thread& y) const {return x.m_priority>y.m_priority;}
};

#endif

//
//  thread.h
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/12/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#ifndef TPSO1_thread_h
#define TPSO1_thread_h

#include <cucontext>

struct Thread_Greater : binary_function <Thread,Thread,bool> {
    bool operator() (const Thread& x, const Thread& y) const {return x.m_priority>y.m_priority;}
};

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
    
    
    
    friend Thread_Greater::operator()(const Thread&, const Thread&);
    friend class Scheduler;
};




#endif

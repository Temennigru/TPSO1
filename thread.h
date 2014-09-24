//
//  thread.h
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 8/12/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//


#ifndef TPSO1_thread_h
#define TPSO1_thread_h

#define _XOPEN_SOURCE
#define STANDARD_STACK_SIZE 2097152

#include <ucontext.h>
#include <functional>
#include <signal.h>

extern ucontext_t* __main_context__;

class Thread {
private:
    
    typedef unsigned char* Stack;
    
    ucontext_t m_context;
    ucontext_t& main_context;
    void(*m_callback)();
    int m_param;
    int m_init_priority;
    int m_priority;
    Stack stack;
    
public:
    Thread(void(*callback)(), int param, int threadId, int priority = 50);
    void exec();
    void save();
    void restore();

    int m_id;
    
    friend struct Thread_Greater;
    friend class __Internal_Scheduler__;
};

typedef Thread* ThreadPtr;

struct Thread_Greater : public std::binary_function <ThreadPtr,ThreadPtr,bool> {
    bool operator() (const ThreadPtr& x, const ThreadPtr& y) const {return x->m_priority>y->m_priority;}
};

#endif

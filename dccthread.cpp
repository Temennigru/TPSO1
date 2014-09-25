//
//  dccthread.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 9/25/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "dccthread.h"
#include "scheduler.h"
#include <cstdlib>
#include <iostream>

void dccthread_init(void (*func)(), int param) {
    Scheduler* s = new Scheduler(func, param);
    std::cout << "Critical error: Main process returned" << std::endl;
    abort();
}

dccthread_t * dccthread_create(const char *name, void(*func)(), int param) {
    Scheduler* s = new Scheduler(func, param);
    return s->main_scheduler->NewThread(func, param);
    
}
void dccthread_yield(void) {
    Scheduler* s = new Scheduler();
    s->main_scheduler->ThreadYield();
}

void dccthread_sleep(struct timespec ts) {
    Scheduler* s = new Scheduler();
    s->main_scheduler->ThreadSleep(ts);
}

void dccthread_exit() {
    Scheduler* s = new Scheduler();
    s->main_scheduler->ThreadExit();
}

void dccthread_wait(dccthread_t* thread) {
    Scheduler* s = new Scheduler();
    s->main_scheduler->ThreadWait(*thread);
}
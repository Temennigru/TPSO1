#ifndef TPSO1_dccthread_H
#define TPSO1_dccthread_H

#include <ucontext.h>
#include "scheduler.h"

typedef int dccthread_t;

void dccthread_init(void (*func)(), int param);
dccthread_t * dccthread_create(const char *name, void(*func)(), int param);
void dccthread_yield(void);




#endif

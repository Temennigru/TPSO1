//
//  util.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 9/24/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "util.h"
#include <sys/time.h>

void alarm (int type, unsigned int useconds, itimerval& timer, bool repeat) {
    timer.it_interval.tv_usec = repeat ? useconds : 0;
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = useconds;
    setitimer (type, &timer, NULL);
}

void alarmDisable (itimerval& timer) {
    timer.it_interval.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
}
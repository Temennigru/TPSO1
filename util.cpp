//
//  util.cpp
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 9/24/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#include "util.h"
#include <sys/time.h>

void alarm (unsigned int useconds) {
    struct itimerval newTimer;
    newTimer.it_interval.tv_usec = 0;
    newTimer.it_interval.tv_sec = 0;
    newTimer.it_value.tv_sec = 0;
    newTimer.it_value.tv_usec = useconds;
    setitimer (ITIMER_REAL, &newTimer, NULL);
}
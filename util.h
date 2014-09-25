//
//  util.h
//  TPSO1
//
//  Created by Jean-Luc Nacif Coelho on 9/24/14.
//  Copyright (c) 2014 GrupoSO. All rights reserved.
//

#ifndef __TPSO1__util__
#define __TPSO1__util__

#define RUNTIME 10

#include <stdio.h>
#include <sys/time.h>


void alarm (int type, unsigned int useconds, itimerval& timer, bool repeat);
void alarmDisable (itimerval& timer);


#endif /* defined(__TPSO1__util__) */

#ifndef THREADER_H
#define THREADER_H

#include <pthread.h>
#include <stdbool.h>
#include "grid.h"

typedef struct {
    bool isThreadRunning;
    bool isThreadEverCreated;
    pthread_t calculationThread;
    pthread_mutex_t lock;
} threader_t; 

threader_t* getThreader();
void* threader_calculateNextStep(void *args);
void threader_startCalculationIfReady();

#endif
#include "threader.h"

static threader_t threader = {0};

threader_t* getThreader() {
    return &threader;
}

void* threader_calculateNextStep(void *args) {
    calculateNextStateMultithreaded();
    threader.isThreadRunning = false;
    return NULL;
}

void threader_startCalculationIfReady() {
    if (threader.isThreadRunning == false) {
        if (threader.isThreadEverCreated) pthread_detach(threader.calculationThread);

        pthread_create(&threader.calculationThread, NULL, threader_calculateNextStep, NULL);

        threader.isThreadRunning = true;
        threader.isThreadEverCreated = true;
    }
}
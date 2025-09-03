#include "threader.h"
#include "grid.h"

static threader_t threader = {0};

void* threader_calculateNextStep(void *args) {
    calculateNextState();
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
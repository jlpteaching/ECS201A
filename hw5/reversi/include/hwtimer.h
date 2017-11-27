#ifndef HWTIMER_H
#define HWTIMER_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint64_t hrtime_t;

typedef struct {
    hrtime_t start;
    hrtime_t end;
    double cpuMHz;
} hwtimer_t;

hrtime_t _rdtsc();

void resetTimer(hwtimer_t* timer);
void initTimer(hwtimer_t* timer);
void startTimer(hwtimer_t* timer);
void stopTimer(hwtimer_t* timer);
uint64_t getTimerTicks(hwtimer_t* timer);
uint64_t getTimerNs(hwtimer_t* timer);

#endif

/*
    This file was adapted from template files provided by David Wood.
    This was sourced from Homework 3 of his CS 758 at UW - Madison.
    http://pages.cs.wisc.edu/~david/courses/cs758/Fall2016/wiki/index.php?n=Main.Homework3

    That, in turn, was based off an assignment by John Mellor-Crummey at Rice University.
*/

#ifndef HWTIMER_H
#define HWTIMER_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint64_t hrtime_t;

typedef struct
{
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

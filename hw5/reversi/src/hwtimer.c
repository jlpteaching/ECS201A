/*
    This file was adapted from template files provided by David Wood.
    This was sourced from Homework 3 of his CS 758 at UW - Madison.
    http://pages.cs.wisc.edu/~david/courses/cs758/Fall2016/wiki/index.php?n=Main.Homework3

    That, in turn, was based off an assignment by John Mellor-Crummey at Rice University.
*/

#include "hwtimer.h"

hrtime_t _rdtsc()
{
    unsigned long int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (hrtime_t) hi << 32 | lo;
}

void resetTimer(hwtimer_t* timer)
{
    timer->start = 0;
    timer->end = 0;
}

void initTimer(hwtimer_t* timer)
{
    #if defined(__linux) || defined(__linux__) || defined(linux)
        FILE* cpuinfo;
        char str[100];

        cpuinfo = fopen("/proc/cpuinfo", "r");

        while (fgets(str, 100, cpuinfo) != NULL)
        {
            char cmp_str[8];
            strncpy(cmp_str, str, 7);
            cmp_str[7] = '\0';

            if (strcmp(cmp_str, "cpu MHz") == 0)
            {
                double cpu_mhz;
                sscanf(str, "cpu MHz : %lf", &cpu_mhz);
                timer->cpuMHz = cpu_mhz;
                break;
            }
        }

        fclose(cpuinfo);
    #else
        timer->cpuMHz = 0;
    #endif

    resetTimer(timer);
}

void startTimer(hwtimer_t* timer)
{
    timer->start = _rdtsc();
}

void stopTimer(hwtimer_t* timer)
{
    timer->end = _rdtsc();
}

uint64_t getTimerTicks(hwtimer_t* timer)
{
    return timer->end - timer->start;
}

uint64_t getTimerNs(hwtimer_t* timer)
{
    if (timer->cpuMHz == 0)
    {
        // Cannot use a timer without first initializing it, or if not on Linux.
        return 0;
    }

    return (uint64_t) (((double) getTimerTicks(timer)) / timer->cpuMHz * 1000);
}

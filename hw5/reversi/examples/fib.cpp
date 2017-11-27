#include <iostream>
#include <cstdlib>
#include <cilk/cilk.h>

#include "hwtimer.h"

using namespace std;

int fib(int a)
{
    if (a==0 || a==1) {
        return a;
    }
    int l = cilk_spawn fib(a-1);
    int r = cilk_spawn fib(a-2);

    cilk_sync;

    return l+r;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "Usage: fib <num>" << endl;
        return 1;
    }

    hwtimer_t timer;
    initTimer(&timer);

    int param = atoi(argv[1]);

    startTimer(&timer);
    int ans = fib(param);
    stopTimer(&timer);

    cout << "fib(" << param << ") = " << ans << endl;
    cout << "Total time: " << getTimerNs(&timer) << "ns" << endl;

    return 0;
}

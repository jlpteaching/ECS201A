#include <iostream>
#include <cilk/cilk.h>

using namespace std;

int global = 0;

void reducer(int i)
{
    global += i;
}

int main(int argc, char* argv[])
{
    cilk_spawn reducer(1);
    cilk_spawn reducer(2);

    cilk_sync;

    cout << "global is " << global << endl;

    return 0;
}

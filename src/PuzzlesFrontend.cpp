
#include "PuzzlesConfig.h"
#include "CGameContainer.h"

#include <sys/time.h>
#include <stdarg.h>
#include <string.h>

#include <e32std.h>


extern "C" {
#include "puzzles.h"
}

extern "C" {

void frontend_default_colour(frontend *fe, float *output) {
    (void)fe;
    
    output[0] = 250. / 255;
    output[1] = 250. / 255;
    output[2] = 250. / 255;
}

void activate_timer(frontend *fe) {
    ((CGameContainer*)fe)->StartTimer();
}

void deactivate_timer(frontend *fe) {
    ((CGameContainer*)fe)->EndTimer();
}

void get_random_seed(void **randseed, int *randseedsize) {
    TInt64 *now = snew(TInt64);
    
    TTime time;
    time.UniversalTime();
    *now = time.Int64();
    
    *randseed = now;
    *randseedsize = sizeof(now);
}

void fatal(char *fmt, ...) {
    char buf[128];
    
    TInt block;
    RHeap &heap = User::Heap();
    TInt avail = heap.Available(block);
    RDebug::Printf(
        "Max: %d, Size: %d, Avail: %d, Large Block: %d",
        (int)heap.MaxLength(),
        (int)heap.Size(),
        (int)avail,
        (int)block
    );
    
    va_list ap;
    
    va_start(ap, fmt);
    vsnprintf(buf, 127, fmt, ap);
    va_end(ap);
    buf[127] = 0;
    
    TBuf<128> panic;
    panic.Copy(TPtrC8((TUint8*)buf));
    
    User::Panic(panic, 0);
}

#ifdef DEBUGGING
void debug_printf(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
}
#endif

#ifndef NDEBUG
static TThreadStackInfo stackInfo;
static void* top = NULL;

void updateStack(void *ptr) {
    if (top == NULL) {
        RThread().StackInfo(stackInfo);
        top = ptr;
        RDebug::Printf(
            "Max: %d, Top: %d", 
            stackInfo.iBase - stackInfo.iLimit,
            stackInfo.iBase - (TLinAddr)top
        );
    }
    else if (ptr < top) {
        top = ptr;
        RDebug::Printf(
            "Max: %d, Top: %d", 
            stackInfo.iBase - stackInfo.iLimit,
            stackInfo.iBase - (TLinAddr)top
        );
    }
}
#endif
}

#pragma once

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG_BUFFER_SIZE 100


#define debug_assert(bTure)     \
{     \
    int nptrs;     \
    void *buffer[DEBUG_BUFFER_SIZE];     \
    if (!(bTure))     \
    {     \
        printf("file: %s---line: %d\n",__FILE__, __LINE__);           \
        nptrs = backtrace(buffer, DEBUG_BUFFER_SIZE);     \
        printf("backtrace() returned %d addresses\n", nptrs);     \
        backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO);     \
        exit(0);\
    }     \
}


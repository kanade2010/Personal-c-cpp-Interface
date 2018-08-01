#ifndef _LOG_HH
#define _LOG_HH

#include <stdio.h>
#include <stdlib.h>

#define CHECK(exp) \
    if(!exp) \
{ \
    fprintf(stderr, "Error/(%s, %d):[" #exp "] check error, abort.\n", __FILE__, __LINE__); abort();\
}

#define VERIFY(exp) \
    if(!exp) \
{ \
    fprintf(stderr, "Error/(%s, %d):[" #exp "] verify error, abort.\n", __FILE__, __LINE__); abort();\
}

#endif
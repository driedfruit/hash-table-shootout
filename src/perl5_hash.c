#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include <stdio.h>
#include <string.h>

#define SETUP \
    int myargc = 1; \
    char * * myargv = argv; \
    PerlInterpreter *my_perl=NULL; \
    PERL_SYS_INIT(&myargc, &myargv); \
    my_perl = perl_alloc(); \
    perl_construct( my_perl ); \
    PERL_SET_CONTEXT(my_perl); \
    HV * hash = newHV(); \
    char p5_int_buffer[sizeof(int)*10];

#define INSERT_INT_INTO_HASH(key, value) do { \
        snprintf(p5_int_buffer, sizeof(p5_int_buffer), "%d", key); \
        hv_store(hash, p5_int_buffer, strlen(p5_int_buffer), &PL_sv_undef, 0); \
    } while(0)

#define DELETE_INT_FROM_HASH(key) do { \
        snprintf(p5_int_buffer, sizeof(p5_int_buffer), "%d", key); \
        hv_delete(hash, p5_int_buffer, strlen(p5_int_buffer), G_DISCARD); \
    } while(0)
#define INSERT_STR_INTO_HASH(key, value) do { \
        hv_store(hash, key, strlen(key), &PL_sv_undef, 0); \
    } while(0)
#define DELETE_STR_FROM_HASH(key) do { \
        hv_delete(hash, key, strlen(key), G_DISCARD); \
    } while(0)
#include "template.c"

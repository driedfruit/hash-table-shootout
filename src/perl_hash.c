#include <EXTERN.h>
#include <perl.h>

typedef HV * hash_t; /* required for template.c */
static PerlInterpreter *my_perl;
static SV * perl_int_key;
static SV * perl_int_value;

#define SETUP \
    my_perl = perl_alloc(); \
    perl_construct(my_perl); \
    hash_t hash = newHV(); \
    perl_int_value = newSViv(0);
#define INSERT_INT_INTO_HASH(key, value) \
    perl_int_key = newSViv(key); \
    hv_store_ent(hash, perl_int_key, perl_int_value, 0);
#define DELETE_INT_FROM_HASH(key) \
    perl_int_key = newSViv(key); \
    hv_delete_ent(hash, perl_int_key, G_DISCARD, 0);
#define INSERT_STR_INTO_HASH(key, value) \
    hv_store(hash, key, strlen(key), perl_int_value, 0);
#define DELETE_STR_FROM_HASH(key) \
    hv_delete(hash, key, strlen(key), G_DISCARD);

#include "template.c"

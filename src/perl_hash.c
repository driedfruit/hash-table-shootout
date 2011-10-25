/*
    This Perl version is modeled after the Python one.
    I also reproduced the "ignore-value-just-pass-zero" optimization
    that the Python version seems to follow.

    This may seem to advantage Python (and thus Perl), and it is
    in fact IMO, but OTOH, the Google sparse/dense hash maps or
    anything C/C++ based has the enormous advantage of using the
    native types, without having to pass through a PyObject or
    a Perl SV.

    So, fair enough?   Cosimo, 01/Apr/2010
*/

#include <EXTERN.h>
#include <perl.h>

typedef HV * hash_t;
static PerlInterpreter *my_perl;
static SV * perl_int_key;
static SV * perl_int_value;

#define HASH_IDENTIFY_SELF printf("perl " PERL_VERSION_STRING " HV hash");

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

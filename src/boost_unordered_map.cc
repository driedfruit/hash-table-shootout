#include <inttypes.h>
#include <boost/unordered_map.hpp>
typedef boost::unordered_map<int64_t, int64_t> hash_t;
typedef boost::unordered_map<const char *, int64_t> str_hash_t;
#define SETUP hash_t hash; str_hash_t str_hash;
#define INSERT_INT_INTO_HASH(key, value) hash.insert(hash_t::value_type(key, value))
#define DELETE_INT_FROM_HASH(key) hash.erase(key)
#define INSERT_STR_INTO_HASH(key, value) str_hash.insert(str_hash_t::value_type(key, value))
#define DELETE_STR_FROM_HASH(key) str_hash.erase(key)
#define HASH_IDENTIFY_SELF printf("Boost 1.38 unordered_map\n") 
#include "template.c"

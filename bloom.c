/**
 * file name  : bloom.c
 * authors    : Dave Pederson
 * created    : Jul 20, 2011
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Jul 20, 2011  Dave Pederson    Creation
 */
#include <limits.h>
#include <stdarg.h>
#include "bloom.h"
#include "hashes.h"

#define SETBIT(bitset, i) (bitset[i / CHAR_BIT] |= (1 << (i % CHAR_BIT)))
#define GETBIT(bitset, i) (bitset[i / CHAR_BIT]  & (1 << (i % CHAR_BIT)))

/**
 * Bloom filter type implementation
 */
struct _bloom_t 
{
    unsigned char *bitset;     // The bitset data structure
    size_t size;               // The size of the bitset
    size_t count;              // The number of keys in the bloom filter
    bloom_hashfunc *functions; // The array of hash function pointers
    size_t num_functions;      // The number of hash function pointers
};

/**
 * Internal bloom filter builder function
 */
bloom_t *bloom_filter_builder(size_t size, size_t num_functions, ...)
{
    bloom_t *filter;
    va_list valist;
    if (!(filter = malloc(sizeof(bloom_t)))) {
        return NULL;
    }
    if (!(filter->bitset = calloc((size + CHAR_BIT-1) / CHAR_BIT, sizeof(char)))) {
        free(filter);
        return NULL;
    }
    if (!(filter->functions = malloc(num_functions * sizeof(bloom_hashfunc)))) {
        free(filter->bitset);
        free(filter);
        return NULL;
    }
    va_start(valist, num_functions);
    int i; for (i = 0; i < num_functions; ++i) {
        filter->functions[i] = va_arg(valist, bloom_hashfunc);
    }
    va_end(valist);
    filter->num_functions = num_functions;
    filter->size = size;
    filter->count = 0;
    return filter;
}

/**
 * Allocate a new bloom filter
 */
bloom_t *bloom_filter_new(size_t size)
{
    return bloom_filter_builder(
        size, 4, jenkins_hash, murmur_hash, sax_hash, sdbm_hash);
}


/**
 * Free and allocated bloom filter
 */
int bloom_filter_free(bloom_t *filter)
{
    if (!filter) {
        return 0;
    }
    if (filter->bitset) {
        free(filter->bitset);
    }
    if (filter->functions) {
        free(filter->functions);
    }
    free(filter);
    return 1;
}

/**
 * Add a key to a bloom filter
 */
int bloom_filter_add(bloom_t *filter, const char *key)
{
    if (!filter || !key) {
        return 0;
    }
    int i; for (i = 0; i < filter->num_functions; ++i) {
        SETBIT(filter->bitset, filter->functions[i](key) % filter->size);
    }
    ++(filter->count);
    return 1;
}

/**
 * Check bloom filter membership
 */
int bloom_filter_contains(bloom_t *filter, const char *key)
{
    if (!filter || !key) {
        return 0;
    }
    int i; for (i = 0; i < filter->num_functions; ++i) {
        if (!(GETBIT(filter->bitset, filter->functions[i](key) % filter->size))) {
            return 0;
        }
    }
    return 1;
}

/**
 * The number of keys in the bloom filter
 */
size_t bloom_filter_count(bloom_t *filter)
{
    return filter->count;
}

/**
 * The size of the bloom filter
 */
size_t bloom_filter_size(bloom_t *filter)
{
    return filter->size;
}

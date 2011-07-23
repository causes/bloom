/**
 * file name  : bloom.h
 * authors    : Dave Pederson
 * created    : Jul 20, 2011
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Jul 20, 2011  Dave Pederson    Creation
 */
#ifndef __BLOOM_H_INCLUDED__
#define __BLOOM_H_INCLUDED__

#include <stdlib.h>

/**
 * Hash function pointer
 */
typedef unsigned int(*bloom_hashfunc)(const char*);

/**
 * Opaque type definition
 */
typedef struct _bloom_t bloom_t;

/**
 * Allocate a new bloom filter
 */
bloom_t *bloom_filter_new(size_t);

/**
 * Free and allocated bloom filter
 */
int bloom_filter_free(bloom_t*);

/**
 * Add a key to a bloom filter
 */
int bloom_filter_add(bloom_t*, const char*);

/**
 * Check bloom filter membership
 */
int bloom_filter_contains(bloom_t*, const char*);

/**
 * The number of keys in the bloom filter
 */
size_t bloom_filter_count(bloom_t*);

/**
 * The size of the bloom filter
 */
size_t bloom_filter_size(bloom_t*);


#endif // __BLOOM_H_INCLUDED__

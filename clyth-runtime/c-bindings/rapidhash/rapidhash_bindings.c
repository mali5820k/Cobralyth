#include <stdint.h>
#include <stddef.h>
#include "vendor/rapidhash/rapidhash.h"

uint64_t clyth_rapidhashu64(const void* data, uint64_t length, uint64_t seed) {
    if (data == NULL && length != 0) {
        return 0;
    }
    return rapidhash_withSeed(data, (size_t)length, seed);
}

uint64_t clyth_rapidhash64(const void* data, uint64_t length, uint64_t seed) {
    return clyth_rapidhashu64(data, length, seed);
}

uint64_t clyth_hash_bytes(const void* data, uint64_t length, uint64_t seed) {
    return clyth_rapidhashu64(data, length, seed);
}

uint64_t clyth_hash_u64(uint64_t value, uint64_t seed) {
    return clyth_rapidhashu64(&value, sizeof(value), seed);
}

uint64_t clyth_hash_i64(int64_t value, uint64_t seed) {
    return clyth_rapidhashu64(&value, sizeof(value), seed);
}

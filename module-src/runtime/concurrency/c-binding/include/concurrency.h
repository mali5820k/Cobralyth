#ifndef CLYTH_RUNTIME_CONCURRENCY_H
#define CLYTH_RUNTIME_CONCURRENCY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*clyth_thread_entry_fn)(void*);

typedef struct clyth_posix_thread clyth_posix_thread;

clyth_posix_thread* clyth_posix_thread_start(clyth_thread_entry_fn entry, void* arg);
int32_t clyth_posix_thread_completed(clyth_posix_thread* thread);
void* clyth_posix_thread_result(clyth_posix_thread* thread);
int32_t clyth_posix_thread_release(clyth_posix_thread* thread);
uint64_t clyth_runtime_worker_thread_count(void);
void clyth_runtime_set_worker_thread_count(uint64_t count);

typedef void (*clyth_work_fn)(void*);
typedef struct clyth_worker_pool clyth_worker_pool;

clyth_worker_pool* clyth_worker_pool_create(uint32_t workers, uint64_t stack_bytes);
int32_t clyth_worker_pool_submit(clyth_worker_pool* pool, clyth_work_fn work, void* arg);
int32_t clyth_worker_pool_pending(clyth_worker_pool* pool);
int32_t clyth_worker_pool_destroy(clyth_worker_pool* pool);

#ifdef __cplusplus
}
#endif

#endif

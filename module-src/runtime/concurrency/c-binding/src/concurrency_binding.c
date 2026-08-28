#include "concurrency.h"

#include <pthread.h>
#include <limits.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <unistd.h>

struct clyth_posix_thread {
    pthread_t thread;
    clyth_thread_entry_fn entry;
    void* arg;
    void* result;
    atomic_int completed;
    atomic_int joined;
};

static atomic_uint_least64_t clyth_worker_thread_count = 0;

#ifndef CLYTH_THREAD_STACK_MIN
#define CLYTH_THREAD_STACK_MIN ((size_t)16384)
#endif


static void* clyth_thread_trampoline(void* opaque) {
    clyth_posix_thread* thread = (clyth_posix_thread*)opaque;
    if (thread == NULL) {
        return NULL;
    }

    void* result = NULL;
    if (thread->entry != NULL) {
        result = thread->entry(thread->arg);
    }

    thread->result = result;
    atomic_store(&thread->completed, 1);
    return result;
}

clyth_posix_thread* clyth_posix_thread_start(clyth_thread_entry_fn entry, void* arg) {
    clyth_posix_thread* thread = (clyth_posix_thread*)calloc(1, sizeof(clyth_posix_thread));
    if (thread == NULL) {
        return NULL;
    }

    thread->entry = entry;
    thread->arg = arg;
    atomic_store(&thread->completed, 0);
    atomic_store(&thread->joined, 0);

    if (pthread_create(&thread->thread, NULL, clyth_thread_trampoline, thread) != 0) {
        free(thread);
        return NULL;
    }

    return thread;
}

int32_t clyth_posix_thread_completed(clyth_posix_thread* thread) {
    if (thread == NULL) {
        return 0;
    }
    return atomic_load(&thread->completed) ? 1 : 0;
}

void* clyth_posix_thread_result(clyth_posix_thread* thread) {
    if (thread == NULL) {
        return NULL;
    }

    if (!atomic_load(&thread->completed)) {
        return NULL;
    }

    if (!atomic_exchange(&thread->joined, 1)) {
        pthread_join(thread->thread, NULL);
    }

    return thread->result;
}

int32_t clyth_posix_thread_release(clyth_posix_thread* thread) {
    if (thread == NULL) {
        return 0;
    }

    if (!atomic_exchange(&thread->joined, 1)) {
        pthread_join(thread->thread, NULL);
    }

    free(thread);
    return 1;
}

uint64_t clyth_runtime_worker_thread_count(void) {
    uint64_t configured = atomic_load(&clyth_worker_thread_count);
    if (configured != 0) {
        return configured;
    }

    long detected = sysconf(_SC_NPROCESSORS_ONLN);
    if (detected <= 0) {
        return 4;
    }

    return (uint64_t)detected;
}

void clyth_runtime_set_worker_thread_count(uint64_t count) {
    atomic_store(&clyth_worker_thread_count, count);
}

typedef struct clyth_work_item {
    clyth_work_fn work;
    void* arg;
    struct clyth_work_item* next;
} clyth_work_item;

struct clyth_worker_pool {
    pthread_t* threads;
    uint32_t thread_count;
    pthread_mutex_t mutex;
    pthread_cond_t ready;
    pthread_cond_t drained;
    clyth_work_item* head;
    clyth_work_item* tail;
    uint64_t pending;
    int stopping;
};

static void* clyth_worker_loop(void* opaque) {
    clyth_worker_pool* pool = (clyth_worker_pool*)opaque;
    for (;;) {
        pthread_mutex_lock(&pool->mutex);
        while (!pool->stopping && pool->head == NULL) pthread_cond_wait(&pool->ready, &pool->mutex);
        if (pool->stopping && pool->head == NULL) { pthread_mutex_unlock(&pool->mutex); break; }
        clyth_work_item* item = pool->head;
        pool->head = item->next;
        if (pool->head == NULL) pool->tail = NULL;
        pthread_mutex_unlock(&pool->mutex);
        item->work(item->arg);
        free(item);
        pthread_mutex_lock(&pool->mutex);
        if (pool->pending > 0) --pool->pending;
        if (pool->pending == 0) pthread_cond_broadcast(&pool->drained);
        pthread_mutex_unlock(&pool->mutex);
    }
    return NULL;
}

clyth_worker_pool* clyth_worker_pool_create(uint32_t workers, uint64_t stack_bytes) {
    if (workers == 0) workers = (uint32_t)clyth_runtime_worker_thread_count();
    if (workers == 0) workers = 1;
    clyth_worker_pool* pool = (clyth_worker_pool*)calloc(1, sizeof(*pool));
    if (!pool) return NULL;
    pool->threads = (pthread_t*)calloc(workers, sizeof(pthread_t));
    if (!pool->threads) { free(pool); return NULL; }
    pool->thread_count = workers;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->ready, NULL);
    pthread_cond_init(&pool->drained, NULL);
    pthread_attr_t attr; pthread_attr_init(&attr);
    if (stack_bytes != 0) {
        size_t min_stack = CLYTH_THREAD_STACK_MIN;
#ifdef PTHREAD_STACK_MIN
        if ((size_t)PTHREAD_STACK_MIN > min_stack) min_stack = (size_t)PTHREAD_STACK_MIN;
#endif
        size_t requested = (size_t)stack_bytes < min_stack ? min_stack : (size_t)stack_bytes;
        (void)pthread_attr_setstacksize(&attr, requested);
    }
    for (uint32_t i=0;i<workers;i++) {
        if (pthread_create(&pool->threads[i], &attr, clyth_worker_loop, pool) != 0) {
            pool->stopping = 1; pthread_cond_broadcast(&pool->ready);
            for (uint32_t j=0;j<i;j++) pthread_join(pool->threads[j], NULL);
            pthread_attr_destroy(&attr); pthread_cond_destroy(&pool->drained); pthread_cond_destroy(&pool->ready); pthread_mutex_destroy(&pool->mutex); free(pool->threads); free(pool); return NULL;
        }
    }
    pthread_attr_destroy(&attr);
    return pool;
}

int32_t clyth_worker_pool_submit(clyth_worker_pool* pool, clyth_work_fn work, void* arg) {
    if (!pool || !work) return 0;
    clyth_work_item* item=(clyth_work_item*)calloc(1,sizeof(*item)); if(!item)return 0;
    item->work=work; item->arg=arg;
    pthread_mutex_lock(&pool->mutex);
    if (pool->stopping) { pthread_mutex_unlock(&pool->mutex); free(item); return 0; }
    if (pool->tail) pool->tail->next=item; else pool->head=item;
    pool->tail=item; ++pool->pending; pthread_cond_signal(&pool->ready); pthread_mutex_unlock(&pool->mutex); return 1;
}
int32_t clyth_worker_pool_pending(clyth_worker_pool* pool) {
    if (!pool) return 0;
    pthread_mutex_lock(&pool->mutex);
    uint64_t n = pool->pending;
    pthread_mutex_unlock(&pool->mutex);
    return n > INT32_MAX ? INT32_MAX : (int32_t)n;
}
int32_t clyth_worker_pool_destroy(clyth_worker_pool* pool) {
    if (!pool) return 1;
    pthread_mutex_lock(&pool->mutex);
    while (pool->pending != 0) pthread_cond_wait(&pool->drained, &pool->mutex);
    pool->stopping = 1;
    pthread_cond_broadcast(&pool->ready);
    pthread_mutex_unlock(&pool->mutex);
    for (uint32_t i = 0; i < pool->thread_count; i++) pthread_join(pool->threads[i], NULL);
    pthread_cond_destroy(&pool->drained);
    pthread_cond_destroy(&pool->ready);
    pthread_mutex_destroy(&pool->mutex);
    free(pool->threads);
    free(pool);
    return 1;
}

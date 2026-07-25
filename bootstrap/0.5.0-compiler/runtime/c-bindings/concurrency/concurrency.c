#include "concurrency.h"

#include <pthread.h>
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

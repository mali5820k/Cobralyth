#ifndef CLYTH_RUNTIME_CONCURRENCY_H
#define CLYTH_RUNTIME_CONCURRENCY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*clyth_thread_entry_fn)(void*);

typedef struct clyth_posix_thread clyth_posix_thread;
typedef struct clyth_mutex clyth_mutex;
typedef struct clyth_condition clyth_condition;
typedef struct clyth_atomic_int64 clyth_atomic_int64;

clyth_posix_thread* clyth_posix_thread_start(clyth_thread_entry_fn entry, void* arg);
int32_t clyth_posix_thread_completed(clyth_posix_thread* thread);
void* clyth_posix_thread_result(clyth_posix_thread* thread);
int32_t clyth_posix_thread_release(clyth_posix_thread* thread);
uint64_t clyth_runtime_worker_thread_count(void);
void clyth_runtime_set_worker_thread_count(uint64_t count);

clyth_mutex* clyth_mutex_create(void);
int32_t clyth_mutex_lock(clyth_mutex* mutex);
int32_t clyth_mutex_unlock(clyth_mutex* mutex);
int32_t clyth_mutex_release(clyth_mutex* mutex);

clyth_condition* clyth_condition_create(void);
int32_t clyth_condition_wait(clyth_condition* condition, clyth_mutex* mutex);
int32_t clyth_condition_signal(clyth_condition* condition);
int32_t clyth_condition_broadcast(clyth_condition* condition);
int32_t clyth_condition_release(clyth_condition* condition);

clyth_atomic_int64* clyth_atomic_int64_create(int64_t value);
int64_t clyth_atomic_int64_load(clyth_atomic_int64* value);
void clyth_atomic_int64_store(clyth_atomic_int64* value, int64_t next);
int64_t clyth_atomic_int64_add(clyth_atomic_int64* value, int64_t amount);
int32_t clyth_atomic_int64_release(clyth_atomic_int64* value);

#ifdef __cplusplus
}
#endif

#endif

/* Generic worker context and bounded message channel used by Clyth-space workers. */
typedef struct clyth_worker_context clyth_worker_context;
typedef struct clyth_message_channel clyth_message_channel;

int64_t clyth_worker_context_create(int64_t integer_slots, int64_t string_slots);
int32_t clyth_worker_context_set_int(int64_t context, int64_t slot, int64_t value);
int64_t clyth_worker_context_get_int(int64_t context, int64_t slot);
int32_t clyth_worker_context_set_string(int64_t context, int64_t slot, const char* value);
const char* clyth_worker_context_get_string(int64_t context, int64_t slot);
int32_t clyth_worker_context_release(int64_t context);

int64_t clyth_message_channel_create(int64_t capacity);
int32_t clyth_message_channel_send(int64_t channel, int32_t kind, int32_t file_index,
                                   int64_t line, int64_t column, const char* text);
int32_t clyth_message_channel_receive(int64_t channel);
int32_t clyth_message_channel_close(int64_t channel);
int32_t clyth_message_channel_kind(int64_t channel);
int32_t clyth_message_channel_file(int64_t channel);
int64_t clyth_message_channel_line(int64_t channel);
int64_t clyth_message_channel_column(int64_t channel);
const char* clyth_message_channel_text(int64_t channel);
int32_t clyth_message_channel_release(int64_t channel);

int64_t clyth_string_trie_create(void);
int32_t clyth_string_trie_insert(int64_t trie, const char* text, int32_t value);
int32_t clyth_string_trie_lookup(int64_t trie, const char* text);
int32_t clyth_string_trie_release(int64_t trie);

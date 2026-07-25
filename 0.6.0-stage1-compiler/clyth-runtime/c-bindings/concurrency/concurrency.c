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

struct clyth_mutex {
    pthread_mutex_t native;
};

struct clyth_condition {
    pthread_cond_t native;
};

struct clyth_atomic_int64 {
    atomic_int_least64_t value;
};

clyth_mutex* clyth_mutex_create(void) {
    clyth_mutex* mutex = (clyth_mutex*)calloc(1, sizeof(clyth_mutex));
    if (mutex == NULL) return NULL;
    if (pthread_mutex_init(&mutex->native, NULL) != 0) {
        free(mutex);
        return NULL;
    }
    return mutex;
}

int32_t clyth_mutex_lock(clyth_mutex* mutex) {
    return mutex == NULL ? -1 : pthread_mutex_lock(&mutex->native);
}

int32_t clyth_mutex_unlock(clyth_mutex* mutex) {
    return mutex == NULL ? -1 : pthread_mutex_unlock(&mutex->native);
}

int32_t clyth_mutex_release(clyth_mutex* mutex) {
    if (mutex == NULL) return 0;
    int result = pthread_mutex_destroy(&mutex->native);
    if (result == 0) free(mutex);
    return result == 0 ? 1 : 0;
}

clyth_condition* clyth_condition_create(void) {
    clyth_condition* condition = (clyth_condition*)calloc(1, sizeof(clyth_condition));
    if (condition == NULL) return NULL;
    if (pthread_cond_init(&condition->native, NULL) != 0) {
        free(condition);
        return NULL;
    }
    return condition;
}

int32_t clyth_condition_wait(clyth_condition* condition, clyth_mutex* mutex) {
    if (condition == NULL || mutex == NULL) return -1;
    return pthread_cond_wait(&condition->native, &mutex->native);
}

int32_t clyth_condition_signal(clyth_condition* condition) {
    return condition == NULL ? -1 : pthread_cond_signal(&condition->native);
}

int32_t clyth_condition_broadcast(clyth_condition* condition) {
    return condition == NULL ? -1 : pthread_cond_broadcast(&condition->native);
}

int32_t clyth_condition_release(clyth_condition* condition) {
    if (condition == NULL) return 0;
    int result = pthread_cond_destroy(&condition->native);
    if (result == 0) free(condition);
    return result == 0 ? 1 : 0;
}

clyth_atomic_int64* clyth_atomic_int64_create(int64_t value) {
    clyth_atomic_int64* atomic_value = (clyth_atomic_int64*)calloc(1, sizeof(clyth_atomic_int64));
    if (atomic_value == NULL) return NULL;
    atomic_init(&atomic_value->value, value);
    return atomic_value;
}

int64_t clyth_atomic_int64_load(clyth_atomic_int64* value) {
    return value == NULL ? 0 : atomic_load(&value->value);
}

void clyth_atomic_int64_store(clyth_atomic_int64* value, int64_t next) {
    if (value != NULL) atomic_store(&value->value, next);
}

int64_t clyth_atomic_int64_add(clyth_atomic_int64* value, int64_t amount) {
    return value == NULL ? 0 : atomic_fetch_add(&value->value, amount) + amount;
}

int32_t clyth_atomic_int64_release(clyth_atomic_int64* value) {
    if (value == NULL) return 0;
    free(value);
    return 1;
}

#include <string.h>

typedef struct clyth_channel_message {
    int32_t kind;
    int32_t file_index;
    int64_t line;
    int64_t column;
    char* text;
} clyth_channel_message;

struct clyth_worker_context {
    int64_t* integers;
    char** strings;
    int64_t integer_count;
    int64_t string_count;
    pthread_mutex_t mutex;
};

struct clyth_message_channel {
    clyth_channel_message* messages;
    int64_t capacity;
    int64_t head;
    int64_t tail;
    int64_t length;
    int closed;
    clyth_channel_message current;
    pthread_mutex_t mutex;
    pthread_cond_t readable;
    pthread_cond_t writable;
};

typedef struct clyth_trie_node {
    int32_t next[128];
    int32_t value;
} clyth_trie_node;

typedef struct clyth_string_trie {
    clyth_trie_node* nodes;
    int64_t length;
    int64_t capacity;
} clyth_string_trie;

static char* clyth_copy_string(const char* value) {
    const char* source = value == NULL ? "" : value;
    size_t size = strlen(source) + 1;
    char* copy = (char*)malloc(size);
    if (copy != NULL) memcpy(copy, source, size);
    return copy;
}

int64_t clyth_worker_context_create(int64_t integer_slots, int64_t string_slots) {
    clyth_worker_context* context = (clyth_worker_context*)calloc(1, sizeof(clyth_worker_context));
    if (context == NULL) return 0;
    context->integer_count = integer_slots;
    context->string_count = string_slots;
    context->integers = (int64_t*)calloc((size_t)integer_slots, sizeof(int64_t));
    context->strings = (char**)calloc((size_t)string_slots, sizeof(char*));
    pthread_mutex_init(&context->mutex, NULL);
    return (int64_t)(intptr_t)context;
}

int32_t clyth_worker_context_set_int(int64_t handle, int64_t slot, int64_t value) {
    clyth_worker_context* context = (clyth_worker_context*)(intptr_t)handle;
    if (context == NULL || slot < 0 || slot >= context->integer_count) return 0;
    pthread_mutex_lock(&context->mutex);
    context->integers[slot] = value;
    pthread_mutex_unlock(&context->mutex);
    return 1;
}

int64_t clyth_worker_context_get_int(int64_t handle, int64_t slot) {
    clyth_worker_context* context = (clyth_worker_context*)(intptr_t)handle;
    if (context == NULL || slot < 0 || slot >= context->integer_count) return 0;
    pthread_mutex_lock(&context->mutex);
    int64_t value = context->integers[slot];
    pthread_mutex_unlock(&context->mutex);
    return value;
}

int32_t clyth_worker_context_set_string(int64_t handle, int64_t slot, const char* value) {
    clyth_worker_context* context = (clyth_worker_context*)(intptr_t)handle;
    if (context == NULL || slot < 0 || slot >= context->string_count) return 0;
    char* copy = clyth_copy_string(value);
    if (copy == NULL) return 0;
    pthread_mutex_lock(&context->mutex);
    free(context->strings[slot]);
    context->strings[slot] = copy;
    pthread_mutex_unlock(&context->mutex);
    return 1;
}

const char* clyth_worker_context_get_string(int64_t handle, int64_t slot) {
    clyth_worker_context* context = (clyth_worker_context*)(intptr_t)handle;
    if (context == NULL || slot < 0 || slot >= context->string_count) return "";
    return context->strings[slot] == NULL ? "" : context->strings[slot];
}

int32_t clyth_worker_context_release(int64_t handle) {
    clyth_worker_context* context = (clyth_worker_context*)(intptr_t)handle;
    if (context == NULL) return 0;
    for (int64_t i = 0; i < context->string_count; ++i) free(context->strings[i]);
    free(context->strings);
    free(context->integers);
    pthread_mutex_destroy(&context->mutex);
    free(context);
    return 1;
}

int64_t clyth_message_channel_create(int64_t capacity) {
    if (capacity < 2) capacity = 2;
    clyth_message_channel* channel = (clyth_message_channel*)calloc(1, sizeof(clyth_message_channel));
    if (channel == NULL) return 0;
    channel->messages = (clyth_channel_message*)calloc((size_t)capacity, sizeof(clyth_channel_message));
    if (channel->messages == NULL) { free(channel); return 0; }
    channel->capacity = capacity;
    pthread_mutex_init(&channel->mutex, NULL);
    pthread_cond_init(&channel->readable, NULL);
    pthread_cond_init(&channel->writable, NULL);
    return (int64_t)(intptr_t)channel;
}

int32_t clyth_message_channel_send(int64_t handle, int32_t kind, int32_t file_index,
                                   int64_t line, int64_t column, const char* text) {
    clyth_message_channel* channel = (clyth_message_channel*)(intptr_t)handle;
    if (channel == NULL) return 0;
    char* copy = clyth_copy_string(text);
    if (copy == NULL) return 0;
    pthread_mutex_lock(&channel->mutex);
    while (channel->length == channel->capacity && !channel->closed) {
        pthread_cond_wait(&channel->writable, &channel->mutex);
    }
    if (channel->closed) { pthread_mutex_unlock(&channel->mutex); free(copy); return 0; }
    clyth_channel_message* message = &channel->messages[channel->tail];
    message->kind = kind;
    message->file_index = file_index;
    message->line = line;
    message->column = column;
    message->text = copy;
    channel->tail = (channel->tail + 1) % channel->capacity;
    channel->length += 1;
    pthread_cond_signal(&channel->readable);
    pthread_mutex_unlock(&channel->mutex);
    return 1;
}

int32_t clyth_message_channel_receive(int64_t handle) {
    clyth_message_channel* channel = (clyth_message_channel*)(intptr_t)handle;
    if (channel == NULL) return 0;
    pthread_mutex_lock(&channel->mutex);
    while (channel->length == 0 && !channel->closed) {
        pthread_cond_wait(&channel->readable, &channel->mutex);
    }
    if (channel->length == 0) { pthread_mutex_unlock(&channel->mutex); return 0; }
    free(channel->current.text);
    channel->current = channel->messages[channel->head];
    memset(&channel->messages[channel->head], 0, sizeof(clyth_channel_message));
    channel->head = (channel->head + 1) % channel->capacity;
    channel->length -= 1;
    pthread_cond_signal(&channel->writable);
    pthread_mutex_unlock(&channel->mutex);
    return 1;
}

int32_t clyth_message_channel_close(int64_t handle) {
    clyth_message_channel* channel = (clyth_message_channel*)(intptr_t)handle;
    if (channel == NULL) return 0;
    pthread_mutex_lock(&channel->mutex);
    channel->closed = 1;
    pthread_cond_broadcast(&channel->readable);
    pthread_cond_broadcast(&channel->writable);
    pthread_mutex_unlock(&channel->mutex);
    return 1;
}
int32_t clyth_message_channel_kind(int64_t h){ clyth_message_channel* c=(clyth_message_channel*)(intptr_t)h; return c?c->current.kind:0; }
int32_t clyth_message_channel_file(int64_t h){ clyth_message_channel* c=(clyth_message_channel*)(intptr_t)h; return c?c->current.file_index:0; }
int64_t clyth_message_channel_line(int64_t h){ clyth_message_channel* c=(clyth_message_channel*)(intptr_t)h; return c?c->current.line:0; }
int64_t clyth_message_channel_column(int64_t h){ clyth_message_channel* c=(clyth_message_channel*)(intptr_t)h; return c?c->current.column:0; }
const char* clyth_message_channel_text(int64_t h){ clyth_message_channel* c=(clyth_message_channel*)(intptr_t)h; return c&&c->current.text?c->current.text:""; }

int32_t clyth_message_channel_release(int64_t handle) {
    clyth_message_channel* channel = (clyth_message_channel*)(intptr_t)handle;
    if (channel == NULL) return 0;
    clyth_message_channel_close(handle);
    for (int64_t i = 0; i < channel->capacity; ++i) free(channel->messages[i].text);
    free(channel->current.text);
    free(channel->messages);
    pthread_cond_destroy(&channel->readable);
    pthread_cond_destroy(&channel->writable);
    pthread_mutex_destroy(&channel->mutex);
    free(channel);
    return 1;
}

static int32_t clyth_trie_append_node(clyth_string_trie* trie) {
    if (trie->length == trie->capacity) {
        int64_t capacity = trie->capacity == 0 ? 32 : trie->capacity * 2;
        clyth_trie_node* nodes = (clyth_trie_node*)realloc(trie->nodes, (size_t)capacity * sizeof(clyth_trie_node));
        if (nodes == NULL) return -1;
        trie->nodes = nodes;
        memset(trie->nodes + trie->capacity, 0, (size_t)(capacity - trie->capacity) * sizeof(clyth_trie_node));
        trie->capacity = capacity;
    }
    int32_t index = (int32_t)trie->length++;
    trie->nodes[index].value = 0;
    return index;
}

int64_t clyth_string_trie_create(void) {
    clyth_string_trie* trie = (clyth_string_trie*)calloc(1, sizeof(clyth_string_trie));
    if (trie == NULL) return 0;
    if (clyth_trie_append_node(trie) < 0) { free(trie); return 0; }
    return (int64_t)(intptr_t)trie;
}

int32_t clyth_string_trie_insert(int64_t handle, const char* text, int32_t value) {
    clyth_string_trie* trie = (clyth_string_trie*)(intptr_t)handle;
    if (trie == NULL || text == NULL) return 0;
    int32_t node = 0;
    for (const unsigned char* cursor = (const unsigned char*)text; *cursor != 0; ++cursor) {
        if (*cursor >= 128) return 0;
        int32_t next = trie->nodes[node].next[*cursor];
        if (next == 0) {
            next = clyth_trie_append_node(trie);
            if (next < 0) return 0;
            trie->nodes[node].next[*cursor] = next;
        }
        node = next;
    }
    trie->nodes[node].value = value;
    return 1;
}

int32_t clyth_string_trie_lookup(int64_t handle, const char* text) {
    clyth_string_trie* trie = (clyth_string_trie*)(intptr_t)handle;
    if (trie == NULL || text == NULL) return 0;
    int32_t node = 0;
    for (const unsigned char* cursor = (const unsigned char*)text; *cursor != 0; ++cursor) {
        if (*cursor >= 128) return 0;
        node = trie->nodes[node].next[*cursor];
        if (node == 0) return 0;
    }
    return trie->nodes[node].value;
}

int32_t clyth_string_trie_release(int64_t handle) {
    clyth_string_trie* trie = (clyth_string_trie*)(intptr_t)handle;
    if (trie == NULL) return 0;
    free(trie->nodes);
    free(trie);
    return 1;
}

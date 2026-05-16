#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "tl_task.h"

const int HALF_WORD_SIZE = 32;

uint64_t high_part_random_uint64_t()
{
    return (uint64_t)rand() << HALF_WORD_SIZE;
}

uint64_t get_random_uint64_t()
{
    return high_part_random_uint64_t() | rand();
}

uint64_t random_id(void)
{
    static bool seeded = false;
    if (!seeded) {
        time_t now = time(NULL); // Pass NULL pointer as to not store results anywhere in memory.
        srand((unsigned)now); // Set starting seed used by rand() to current time
        seeded = true;
    }
    return get_random_uint64_t();
}

// Create a task somewhere in memory and return the address of that memory location.
TlTask *create_task(TlTaskFn fn, void *ctx)
{
    if (fn == NULL)
    {
        return NULL;
    }

    TlTask *task = malloc(sizeof(TlTask));
    task->id = random_id();
    task->fn = fn;
    task->ctx = ctx;
    task->state = CREATED;
    return task;
}

void run_task(TlTask *task)
{
    if (task == NULL || task->state != CREATED)
    {
        return;
    }

    task->state = RUNNING;
    task->fn(task->ctx);
    task->state = DONE;
}

void destroy_task(TlTask *task) {
    free(task);
}



#ifndef TASKLAB_TASK_H
#define TASKLAB_TASK_H

#include <stdint.h>

typedef void (*TlTaskFn)(void *ctx);

enum TlTaskState {
    CREATED,
    RUNNING,
    DONE,
    FAILED,
};

typedef struct TlTask {
    TlTaskFn fn;
    uint64_t id;
    void *ctx;
    enum TlTaskState state;
} TlTask;


TlTask *create_task(TlTaskFn fn, void *ctx);

void run_task(TlTask *task);

void destroy_task(TlTask *task);

#endif //TASKLAB_TASK_H

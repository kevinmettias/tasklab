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


TlTask *Create_And_Initialize_Task(TlTaskFn fn, void *ctx);

void Run_Task(TlTask *task);

void Destroy_Task(TlTask *task);

#endif //TASKLAB_TASK_H

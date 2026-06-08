#ifndef TASKLAB_TASK_H
#define TASKLAB_TASK_H

#include <stdint.h>

typedef void (*TaskFn)(void *ctx);

enum TaskState {
    CREATED,
    RUNNING,
    DONE,
    FAILED,
};

typedef struct Task {
    TaskFn fn;
    uint64_t id;
    void *ctx;
    enum TaskState state;
} Task;


Task *Create_And_Initialize_Task(TaskFn fn, void *ctx);

void Run_Task(Task *task);

void Destroy_Task(Task *task);

#endif //TASKLAB_TASK_H

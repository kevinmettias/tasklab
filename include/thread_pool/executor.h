#ifndef TASKLAB_EXECUTOR_H
#define TASKLAB_EXECUTOR_H

#include "queue.h"

typedef void (*TaskFailurePolicy)(void *ctx);

typedef struct TaskExecutor {
    TaskQueue *queue;
    TaskFailurePolicy failure_fn;
    void *failure_ctx;
} TaskExecutor;


TaskExecutor *Create_And_Initialize_Task_Executor(TaskQueue *queue);

TaskExecutor *Create_And_Initialize_Task_Executor_With_New_Queue();

void Submit_To_Executor(TaskExecutor *executor, Task *task);

void Execute_Tasks_In_TaskQueue_Until_Queue_Empty(TaskExecutor *executor);

void Destroy_Task_Executor(TaskExecutor *executor);

#endif //TASKLAB_EXECUTOR_H

#ifndef TASKLAB_TEST_TASK_EXECUTOR_H
#define TASKLAB_TEST_TASK_EXECUTOR_H

#include <stdbool.h>

#include "tl/task.h"
#include "tl/queue.h"
#include "tl/executor.h"

#ifndef TASKLAB_TEST_CASE_DEFINED
#define TASKLAB_TEST_CASE_DEFINED

typedef struct {
    TlTaskFn fn;
    void *ctx;
} TestCase;
#endif

#ifndef TASKLAB_TEST_CTX_DEFINED
#define TASKLAB_TEST_CTX_DEFINED

typedef struct {
    int input;
    bool was_called;
    int result;
} TestCtx;
#endif

void Test_Executor_Task_Fn(void *ctx);
void Test_Create_And_Initialize_Task_Executor(void **state);
void Test_Create_And_Initialize_Task_Executor_With_New_Queue(void **state);
void Test_Execute_Tasks_In_TaskQueue_Until_Queue_Empty(void **state);

#endif //TASKLAB_TEST_TASK_EXECUTOR_H


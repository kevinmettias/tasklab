#ifndef TASKLAB_TEST_TASK_H
#define TASKLAB_TEST_TASK_H

#include <stdbool.h>

#include "tl/task.h"

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

void Test_Task_Fn(void *ctx);
void Test_Assert_Null_Safe(TlTask *task);
void Test_Create_And_Initialize_Task_Transitions_State_To_Created(void **state);
void Test_Run_Task_Transitions_State_To_Done(void **state);

#endif //TASKLAB_TEST_TASK_H

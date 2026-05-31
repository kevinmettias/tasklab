#ifndef TASKLAB_TEST_SUPPORT_H
#define TASKLAB_TEST_SUPPORT_H

#include <stdbool.h>

#include "tl/task.h"

typedef struct TestCase
{
    TaskFn fn;
    void *ctx;
} TestCase;

typedef struct TestCtx
{
    int input;
    bool was_called;
    int result;
} TestCtx;

void Test_Task_Fn(void *ctx);
void Test_Node_Task_Fn(void *ctx);
void Test_Queue_Task_Fn(void *ctx);
void Test_Executor_Task_Fn(void *ctx);

#endif

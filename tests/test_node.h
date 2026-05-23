#ifndef TASKLAB_TEST_TASK_NODE_H
#define TASKLAB_TEST_TASK_NODE_H

#include "tl/queue.h"
#include <stdbool.h>

typedef struct {
    TlTaskFn fn;
    void *ctx;
} TestCase;

typedef struct {
    int input;
    bool was_called;
    int result;
} TestCtx;

void Test_Node_Task_Fn(void *ctx);

void Test_Create_And_Initialize_TaskNode(void **state);

#endif //TASKLAB_TEST_TASK_NODE_H

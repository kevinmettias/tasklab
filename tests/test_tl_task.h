#ifndef TASKLAB_TEST_TL_TASK_H
#define TASKLAB_TEST_TL_TASK_H

#include "tl/task.h"

typedef struct {
    TlTaskFn fn;
    void *ctx;
} TestCase;

typedef struct {
    int input;
    int was_called;
    int result;
} TestCtx;

void test_task_fn(void *ctx);

void test_create_task_transition_state_to_create(void **state);

void test_run_task_transition_state_to_done(void **state);

#endif //TASKLAB_TEST_TL_TASK_H

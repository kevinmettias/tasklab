#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_node.h"
#include "../src/task_node.h"

void Test_Node_Task_Fn(void *ctx) {
    if (ctx == NULL)
    {
        return;
    }

    TestCtx *test_ctx = ctx;
    test_ctx->was_called = true;
    test_ctx->result = test_ctx->input * 2;
}

void Test_Create_And_Initialize_TaskNode(void **state) {
    TestCase *test_case = *state;

    TlTask *task = Create_And_Initialize_Task(test_case->fn, test_case->ctx);
    if (test_case->fn == NULL)
    {
        assert_null(Create_And_Initialize_TaskNode(task));
        Destroy_Task(task);
        return;
    }

    assert_non_null(task);

    TaskNode *task_node = Create_And_Initialize_TaskNode(task);
    assert_non_null(task_node);
    assert_ptr_equal(task_node->task, task);

    Destroy_TaskNode(task_node);
    Destroy_Task(task);
}

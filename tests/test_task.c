#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_task.h"

void Test_Task_Fn(void *ctx) {
    if (ctx == NULL)
    {
        return;
    }

    TestCtx *test_ctx = ctx;

    test_ctx->was_called = true;
    test_ctx->result = test_ctx->input * 2;
}

void Test_Assert_Null_Safe(TlTask *task)
{
    if (task != NULL)
    {
        Destroy_Task(task);
        task = NULL;
    }
    assert_null(task);
}

void Test_Create_And_Initialize_Task_Transitions_State_To_Created(void **state) {
    TestCase *test_case = *state;

    TlTask *task = Create_And_Initialize_Task(test_case->fn, test_case->ctx);
    if (test_case->fn == NULL)
    {
        Test_Assert_Null_Safe(task);
        return;
    }

    assert_non_null(task);
    assert_int_not_equal(task->id, 0);
    assert_ptr_equal(task->fn, test_case->fn);
    assert_ptr_equal(task->ctx, test_case->ctx);
    assert_int_equal(task->state, CREATED);
    Destroy_Task(task);
}

void Test_Run_Task_Transitions_State_To_Done(void **state) {
    TestCase *test_case = *state;

    TlTask *task = Create_And_Initialize_Task(test_case->fn, test_case->ctx);
    if (test_case->fn == NULL)
    {
        Test_Assert_Null_Safe(task);
        return;
    }

    Run_Task(task);

    if (test_case->ctx != NULL)
    {
        TestCtx *test_ctx = test_case->ctx;
        assert_true(test_ctx->was_called);
        assert_int_equal(test_ctx->result, test_ctx->input * 2);
    }

    assert_int_equal(task->state, DONE);
    Destroy_Task(task);
}

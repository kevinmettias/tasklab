#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_tl_task.h"

void test_task_fn(void *ctx) {
    if (ctx == NULL)
    {
        return;
    }

    TestCtx *test_ctx = ctx;

    test_ctx->was_called = 1;
    test_ctx->result = test_ctx->input * 2;
}

void test_assert_null_safe(TlTask *task)
{
    if (task != NULL)
    {
        destroy_task(task);
        task = NULL;
    }
    assert_null(task);
}

void test_create_task_transition_state_to_create(void **state) {
    TestCase *test_case = *state;

    TlTask *task = create_task(test_case->fn, test_case->ctx);
    if (test_case->fn == NULL)
    {
        test_assert_null_safe(task);
        return;
    }

    assert_non_null(task);
    assert_int_not_equal(task->id, 0);
    assert_ptr_equal(task->fn, test_case->fn);
    assert_ptr_equal(task->ctx, test_case->ctx);
    assert_int_equal(task->state, CREATED);
    destroy_task(task);
}

void test_run_task_transition_state_to_done(void **state) {
    TestCase *test_case = *state;

    TlTask *task = create_task(test_case->fn, test_case->ctx);
    if (test_case->fn == NULL)
    {
        test_assert_null_safe(task);
        return;
    }

    run_task(task);

    if (test_case->ctx != NULL)
    {
        TestCtx *test_ctx = test_case->ctx;
        assert_int_equal(test_ctx->was_called, 1);
        assert_int_equal(test_ctx->result, test_ctx->input * 2);
    }

    assert_int_equal(task->state, DONE);
    destroy_task(task);
}
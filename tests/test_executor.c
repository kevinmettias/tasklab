#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_executor.h"

void Test_Executor_Task_Fn(void *ctx) {
    if (ctx == NULL) {
        return;
    }

    TestCtx *test_ctx = ctx;
    test_ctx->was_called = true;
    test_ctx->result = test_ctx->input * 2;
}

static void Test_Executor_Assert_Null_Safe_Task(Task *task) {
    if (task != NULL) {
        Destroy_Task(task);
    }
    assert_null(task);
}

void Test_Create_And_Initialize_Task_Executor(void **state) {
    TestCase *test_case = *state;

    TaskQueue *queue = Create_And_Initalize_TaskQueue();
    assert_non_null(queue);

    TaskExecutor *executor = Create_And_Initialize_Task_Executor(queue);
    assert_non_null(executor);
    assert_ptr_equal(executor->queue, queue);

    if (test_case->fn != NULL) {
        Task *task = Create_And_Initialize_Task(test_case->fn, test_case->ctx);
        assert_non_null(task);
        Submit_To_Executor(executor, task);
        Execute_Tasks_In_TaskQueue_Until_Queue_Empty(executor);
        assert_int_equal(task->state, DONE);
        Destroy_Task(task);
    }

    Destroy_Task_Executor(executor);
}

void Test_Create_And_Initialize_Task_Executor_With_New_Queue(void **state) {
    (void)state;

    /*TaskExecutor *executor = Create_And_Initialize_Task_Executor_With_New_Queue();
    assert_non_null(executor);
    assert_non_null(executor->queue);
    Destroy_Task_Executor(executor);*/
}

void Test_Execute_Tasks_In_TaskQueue_Until_Queue_Empty(void **state) {
    TestCase *test_case = *state;

    TaskQueue *queue = Create_And_Initalize_TaskQueue();
    assert_non_null(queue);

    TaskExecutor *executor = Create_And_Initialize_Task_Executor(queue);
    assert_non_null(executor);

    if (test_case->fn == NULL) {
        Task *task = Create_And_Initialize_Task(test_case->fn, test_case->ctx);
        Test_Executor_Assert_Null_Safe_Task(task);
        Destroy_Task_Executor(executor);
        return;
    }

    Task *task = Create_And_Initialize_Task(test_case->fn, test_case->ctx);
    assert_non_null(task);

    Submit_To_Executor(executor, task);
    Execute_Tasks_In_TaskQueue_Until_Queue_Empty(executor);

    if (test_case->ctx != NULL) {
        TestCtx *test_ctx = test_case->ctx;
        assert_true(test_ctx->was_called);
        assert_int_equal(test_ctx->result, test_ctx->input * 2);
    }

    assert_int_equal(task->state, DONE);
    Destroy_Task(task);
    Destroy_Task_Executor(executor);
}


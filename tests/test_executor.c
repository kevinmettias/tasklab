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

typedef struct ExecutorOrderCtx {
    int *next_order;
    int expected_order;
    int was_called;
} ExecutorOrderCtx;

static void Test_Executor_Order_Task_Fn(void *ctx) {
    ExecutorOrderCtx *order_ctx = ctx;
    assert_non_null(order_ctx);
    assert_non_null(order_ctx->next_order);

    assert_int_equal(*order_ctx->next_order, order_ctx->expected_order);
    order_ctx->was_called = 1;
    (*order_ctx->next_order)++;
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

void Test_Execute_Empty_TaskQueue_Is_Safe(void **state) {
    (void)state;

    TaskExecutor *executor = Create_And_Initialize_Task_Executor_With_New_Queue();
    assert_non_null(executor);
    assert_non_null(executor->queue);
    assert_int_equal(Get_TaskQueue_Node_Count(executor->queue), 0);

    Execute_Tasks_In_TaskQueue_Until_Queue_Empty(executor);

    assert_int_equal(Get_TaskQueue_Node_Count(executor->queue), 0);
    Destroy_Task_Executor(executor);
}

void Test_Execute_Multiple_Tasks_In_Submission_Order(void **state) {
    (void)state;

    TaskExecutor *executor = Create_And_Initialize_Task_Executor_With_New_Queue();
    assert_non_null(executor);

    int next_order = 0;
    ExecutorOrderCtx ctxs[] = {
        { .next_order = &next_order, .expected_order = 0, .was_called = 0 },
        { .next_order = &next_order, .expected_order = 1, .was_called = 0 },
        { .next_order = &next_order, .expected_order = 2, .was_called = 0 },
    };

    Task *tasks[] = {
        Create_And_Initialize_Task(Test_Executor_Order_Task_Fn, &ctxs[0]),
        Create_And_Initialize_Task(Test_Executor_Order_Task_Fn, &ctxs[1]),
        Create_And_Initialize_Task(Test_Executor_Order_Task_Fn, &ctxs[2]),
    };

    for (size_t i = 0; i < sizeof(tasks) / sizeof(tasks[0]); ++i) {
        assert_non_null(tasks[i]);
        Submit_To_Executor(executor, tasks[i]);
    }

    Execute_Tasks_In_TaskQueue_Until_Queue_Empty(executor);

    assert_int_equal(next_order, 3);
    assert_int_equal(Get_TaskQueue_Node_Count(executor->queue), 0);
    for (size_t i = 0; i < sizeof(tasks) / sizeof(tasks[0]); ++i) {
        assert_true(ctxs[i].was_called);
        assert_int_equal(tasks[i]->state, DONE);
        Destroy_Task(tasks[i]);
    }

    Destroy_Task_Executor(executor);
}

void Test_Create_And_Initialize_Task_Executor_With_New_Queue(void **state) {
    (void)state;

    TaskExecutor *executor = Create_And_Initialize_Task_Executor_With_New_Queue();
    assert_non_null(executor);
    assert_non_null(executor->queue);
    Destroy_Task_Executor(executor);
}

void Test_Execute_Tasks_In_TaskQueue_Until_Queue_Empty(void **state) {
    TestCase *test_case = *state;

    TaskQueue *queue = Create_And_Initalize_TaskQueue();
    assert_non_null(queue);

    TaskExecutor *executor = Create_And_Initialize_Task_Executor(queue);
    assert_non_null(executor);

    if (test_case->fn == NULL) {
        // TODO: WHY DO WE DO THIS?
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


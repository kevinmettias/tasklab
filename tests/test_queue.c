#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_queue.h"
#include "test_node.h"
#include "thread_pool/queue.h"

void Test_Queue_Task_Fn(void *ctx) {
    if (ctx == NULL) {
        return;
    }

    TestCtx *test_ctx = ctx;
    test_ctx->was_called = true;
    test_ctx->result = test_ctx->input * 2;
}

void Test_Create_And_Initalize_TaskQueue(void **state) {
    (void)state;

    TaskQueue *queue = Create_And_Initalize_TaskQueue();
    assert_non_null(queue);
    assert_int_equal(Get_TaskQueue_Node_Count(queue), 0);
    Destroy_TaskQueue(queue);
}

void Test_Push_To_End_Of_TaskQueue(void **state) {
    (void)state;

    TaskQueue *queue = Create_And_Initalize_TaskQueue();
    assert_non_null(queue);

    Task *task1 = Create_And_Initialize_Task(Test_Queue_Task_Fn, NULL);
    Task *task2 = Create_And_Initialize_Task(Test_Queue_Task_Fn, NULL);
    Task *task3 = Create_And_Initialize_Task(Test_Queue_Task_Fn, NULL);

    Push_To_End_Of_TaskQueue(queue, task1);
    Push_To_End_Of_TaskQueue(queue, task2);
    Push_To_End_Of_TaskQueue(queue, task3);

    assert_int_equal(Get_TaskQueue_Node_Count(queue), 3);

    Destroy_Task(task1);
    Destroy_Task(task2);
    Destroy_Task(task3);
    Destroy_TaskQueue(queue);
}

void Test_Create_And_Initalize_TaskQueue_And_Pop_From_Top_Of_TaskQueue(void **state) {
    (void)state;

    TaskQueue *queue = Create_And_Initalize_TaskQueue();
    assert_non_null(queue);

    Task *task1 = Create_And_Initialize_Task(Test_Queue_Task_Fn, NULL);
    Task *task2 = Create_And_Initialize_Task(Test_Queue_Task_Fn, NULL);

    Push_To_End_Of_TaskQueue(queue, task1);
    Push_To_End_Of_TaskQueue(queue, task2);

    assert_int_equal(Get_TaskQueue_Node_Count(queue), 2);

    TaskNode *node1 = Pop_From_Top_Of_TaskQueue(queue);
    assert_non_null(node1);
    assert_ptr_equal(node1->task, task1);

    TaskNode *node2 = Pop_From_Top_Of_TaskQueue(queue);
    assert_non_null(node2);
    assert_ptr_equal(node2->task, task2);

    assert_int_equal(Get_TaskQueue_Node_Count(queue), 0);

    Destroy_Task(task1);
    Destroy_Task(task2);
    Destroy_TaskNode(node1);
    Destroy_TaskNode(node2);
    Destroy_TaskQueue(queue);
}

void Test_Pop_Empty_TaskQueue(void **state) {
    (void)state;

    TaskQueue *queue = Create_And_Initalize_TaskQueue();
    assert_non_null(queue);

    TaskNode *node = Pop_From_Top_Of_TaskQueue(queue);
    assert_null(node);

    Destroy_TaskQueue(queue);
}

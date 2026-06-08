#ifndef TASKLAB_TEST_TASK_EXECUTOR_H
#define TASKLAB_TEST_TASK_EXECUTOR_H

#include "thread_pool/queue.h"
#include "thread_pool/executor.h"
#include "test_support.h"

void Test_Executor_Task_Fn(void *ctx);
void Test_Create_And_Initialize_Task_Executor(void **state);
void Test_Create_And_Initialize_Task_Executor_With_New_Queue(void **state);
void Test_Execute_Tasks_In_TaskQueue_Until_Queue_Empty(void **state);

#endif //TASKLAB_TEST_TASK_EXECUTOR_H


#ifndef TASKLAB_TEST_TASK_QUEUE_H
#define TASKLAB_TEST_TASK_QUEUE_H
#include "test_support.h"
#include "tl/queue.h"
void Test_Queue_Task_Fn(void *ctx);
void Test_Queue_Create_And_Initialize_Task_Transitions_State_To_Created(void **state);
void Test_Queue_Run_Task_Transitions_State_To_Done(void **state);
#endif //TASKLAB_TEST_TASK_QUEUE_H
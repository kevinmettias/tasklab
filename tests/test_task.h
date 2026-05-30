#ifndef TASKLAB_TEST_TASK_H
#define TASKLAB_TEST_TASK_H
#include "test_support.h"
void Test_Task_Fn(void *ctx);
void Test_Assert_Null_Safe(TlTask *task);
void Test_Create_And_Initialize_Task_Transitions_State_To_Created(void **state);
void Test_Run_Task_Transitions_State_To_Done(void **state);
#endif //TASKLAB_TEST_TASK_H
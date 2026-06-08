#ifndef TASKLAB_TEST_TASK_QUEUE_H
#define TASKLAB_TEST_TASK_QUEUE_H

#include "thread_pool/queue.h"
#include "test_support.h"

void Test_Create_And_Initalize_TaskQueue(void **state);
void Test_Push_To_End_Of_TaskQueue(void **state);
void Test_Create_And_Initalize_TaskQueue_And_Pop_From_Top_Of_TaskQueue(void **state);
void Test_Pop_Empty_TaskQueue(void **state);

#endif //TASKLAB_TEST_TASK_QUEUE_H

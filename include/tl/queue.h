#ifndef TASKLAB_QUEUE_H
#define TASKLAB_QUEUE_H

#include <stdint.h>

#include "task.h"

typedef struct TaskNode {
    Task *task;
    struct TaskNode *next;
} TaskNode;

typedef struct TaskQueue {
    uint64_t id;
    TaskNode *first_task_node;
    TaskNode *last_task_node;
} TaskQueue;

TaskQueue *Create_And_Initalize_TaskQueue();

void Push_To_End_Of_TaskQueue(TaskQueue *queue, Task *task);

TaskNode *Pop_From_Top_Of_TaskQueue(TaskQueue *queue);

uint64_t Get_TaskQueue_Node_Count(TaskQueue *queue);

uint64_t Get_TaskQueue_Owned_Structural_Byte_Size(TaskQueue *queue);

void Destroy_TaskQueue(TaskQueue *queue);

#endif //TASKLAB_QUEUE_H

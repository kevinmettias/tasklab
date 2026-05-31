#include <stdlib.h>

#include "task_node.h"
#include "rand_id_generator.h"

TaskQueue *Create_TaskQueue()
{
    return malloc(sizeof(TaskQueue));
}

TaskQueue *Create_And_Initalize_TaskQueue()
{
    TaskQueue *queue = Create_TaskQueue();
    if (queue == NULL)
    {
        return NULL;
    }

    queue->id = Random_uint64_t_Id();
    queue->first_task_node = NULL;
    queue->last_task_node = NULL;
    return queue;
}

void Push_To_End_Of_TaskQueue(TaskQueue *queue, Task *task)
{
    if (task == NULL || task->state != CREATED)
    {
        return;
    }

    if (queue == NULL)
    {
        return;
    }

    TaskNode *new_task_node = Create_And_Initialize_TaskNode(task);
    if (queue->last_task_node != NULL)
    {
        queue->last_task_node->next = new_task_node;
    }

    queue->last_task_node = new_task_node;

    if (queue->first_task_node == NULL)
    {
        queue->first_task_node = new_task_node;
    }
}

TaskNode *Pop_From_Top_Of_TaskQueue(TaskQueue *queue)
{
    if (queue == NULL || queue->first_task_node == NULL)
    {
        return NULL;
    }

    TaskNode *first_node = queue->first_task_node;
    queue->first_task_node = queue->first_task_node->next;
    if (queue->first_task_node == NULL)
    {
        queue->last_task_node = NULL;
    }
    return first_node;
}

uint64_t Get_TaskQueue_Node_Count(TaskQueue *queue)
{
    if (queue == NULL)
    {
        return 0;
    }

    uint64_t count = 0;
    TaskNode *current_node = queue->first_task_node;
    while (current_node != NULL)
    {
        count++;
        current_node = current_node->next;
    }

    return count;
}

uint64_t Get_TaskQueue_Owned_Structural_Byte_Size(TaskQueue *queue)
{
    if (queue == NULL)
    {
        return 0;
    }

    uint64_t byte_size = sizeof(*queue);

    TaskNode *current_node = queue->first_task_node;
    while (current_node != NULL)
    {
        byte_size += sizeof(*current_node);
        current_node = current_node->next;
    }

    return byte_size;
}

void Destroy_TaskQueue(TaskQueue *queue)
{
    if (queue == NULL)
    {
        return;
    }

    TaskNode *current_node = queue->first_task_node;
    while (current_node != NULL)
    {
        TaskNode *next_node = current_node->next;
        Destroy_TaskNode(current_node);
        current_node = next_node;
    }

    free(queue);
}


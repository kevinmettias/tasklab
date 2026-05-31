#include <stdlib.h>

#include "tl/executor.h"

TaskExecutor *Create_Task_Executor()
{
    return malloc(sizeof(TaskExecutor));
}

TaskExecutor *Create_And_Initialize_Task_Executor(TaskQueue *queue)
{
    TaskExecutor *executor = Create_Task_Executor();
    if (executor == NULL || queue == NULL)
    {
        return NULL;
    }

    executor->queue = queue;
    executor->failure_fn = NULL;
    executor->failure_ctx = NULL;
    return executor;
}

TaskExecutor *Create_And_Initialize_Task_Executor_With_New_Queue()
{
    TaskQueue *queue = Create_And_Initalize_TaskQueue();
    return Create_And_Initialize_Task_Executor(queue);
}

void Submit_To_Executor(TaskExecutor *executor, Task *task)
{
    if (executor == NULL)
    {
        return;
    }

    Push_To_End_Of_TaskQueue(executor->queue, task);
}

void Run_Task_In_TaskNode(TaskNode *take_node)
{
    if (take_node == NULL)
    {
        return;
    }

    Task *task = take_node->task;
    Run_Task(task);
}

TaskNode *Execute_Next_Task_In_TaskQueue(TaskExecutor *executor)
{
    TaskQueue *queue = executor->queue;
    TaskNode *task_node = Pop_From_Top_Of_TaskQueue(queue);
    Run_Task_In_TaskNode(task_node);
    if (task_node->task->state == FAILED)
    {
        if (executor->failure_fn != NULL)
        {
            executor->failure_fn(executor->failure_ctx);
        }
    }
    return queue->first_task_node;
}

void Execute_Tasks_In_TaskQueue_Until_Queue_Empty(TaskExecutor *executor)
{
    if (executor == NULL || executor->queue == NULL)
    {
        return;
    }

    while (Execute_Next_Task_In_TaskQueue(executor) != NULL) { }
}

void Destroy_Task_Executor(TaskExecutor *executor)
{
    if (executor == NULL)
    {
        return;
    }

    Destroy_TaskQueue(executor->queue);
    free(executor);
}


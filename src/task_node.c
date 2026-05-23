#include <stdlib.h>

#include "task_node.h"

TaskNode *Create_TaskNode()
{
    return malloc(sizeof(TaskNode));
}

TaskNode *Create_And_Initialize_TaskNode(TlTask *task)
{
    if (task == NULL)
    {
        return NULL;
    }

    TaskNode *node = Create_TaskNode();
    if (node == NULL)
    {
        return NULL;
    }

    node->task = task;
    node->next = NULL;
    return node;
}

void Destroy_TaskNode(TaskNode *node)
{
    if (node == NULL)
    {
        return;
    }

    free(node);
}



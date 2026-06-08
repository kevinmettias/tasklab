#ifndef TASKLAB_NODE_H
#define TASKLAB_NODE_H

#include <stdint.h>

#include "task.h"

typedef struct TaskNode {
    Task *task;
    struct TaskNode *next;
} TaskNode;

void Destroy_TaskNode(TaskNode *node);

#endif //TASKLAB_NODE_H

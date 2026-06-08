#include <stdlib.h>

#include "thread_pool/task.h"
#include "rand_id_generator.h"

Task *Create_Task()
{
    return malloc(sizeof(Task));
}

Task *Create_And_Initialize_Task(TaskFn fn, void *ctx)
{
    if (fn == NULL)
    {
        return NULL;
    }

    Task *task = Create_Task();
    if (task == NULL)
    {
        return NULL;
    }

    task->id = Random_uint64_t_Id();
    task->fn = fn;
    task->ctx = ctx;
    task->state = CREATED;
    return task;
}

void Run_Task(Task *task)
{
    if (task == NULL || task->state != CREATED)
    {
        // TODO: SHOULD WE RETURN RESULT TYPE?
        return;
    }

    task->state = RUNNING;
    task->fn(task->ctx);
    task->state = DONE;
}

void Destroy_Task(Task *task) {
    if (task == NULL)
    {
        return;
    }
    free(task);
}



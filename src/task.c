#include <stdlib.h>

#include "tl/task.h"
#include "rand_id_generator.h"

TlTask *Create_Task()
{
    TlTask *task = malloc(sizeof(TlTask));
    task->id = Random_uint64_t_Id();
    return task;
}

TlTask *Create_And_Initialize_Task(TlTaskFn fn, void *ctx)
{
    if (fn == NULL)
    {
        return NULL;
    }

    TlTask *task = Create_Task();
    task->fn = fn;
    task->ctx = ctx;
    task->state = CREATED;
    return task;
}

void Run_Task(TlTask *task)
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

void Destroy_Task(TlTask *task) {
    if (task == NULL)
    {
        return;
    }
    free(task);
}



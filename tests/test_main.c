#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "test_task.h"
#include "test_node.h"
#include "test_queue.h"
#include "test_executor.h"

#define TEST_ENTRY(name) { #name, name }
#define TEST_TEMPLATE_GROUP_ENTRY(name) { name, sizeof(name)/sizeof(name[0]) }

typedef struct {
    const char *name;
    CMUnitTestFunction test_func;
} TestTemplate;

typedef struct {
    const TestTemplate *array;
    size_t size;
} TestTemplateGroup;

static const TestCtx default_ctx = { .input = 5 };

static const TestTemplate test_task_templates[] = {
    TEST_ENTRY(Test_Create_And_Initialize_Task_Transitions_State_To_Created),
    TEST_ENTRY(Test_Run_Task_Transitions_State_To_Done),
};

static const TestTemplate test_node_templates[] = {
    TEST_ENTRY(Test_Create_And_Initialize_TaskNode),
};

static const TestTemplate test_queue_templates[] = {
    TEST_ENTRY(Test_Create_And_Initalize_TaskQueue),
    TEST_ENTRY(Test_Push_To_End_Of_TaskQueue),
    TEST_ENTRY(Test_Create_And_Initalize_TaskQueue_And_Pop_From_Top_Of_TaskQueue),
    TEST_ENTRY(Test_Pop_Empty_TaskQueue),
};

static const TestTemplate test_executor_templates[] = {
    TEST_ENTRY(Test_Create_And_Initialize_Task_Executor),
    TEST_ENTRY(Test_Create_And_Initialize_Task_Executor_With_New_Queue),
    TEST_ENTRY(Test_Execute_Tasks_In_TaskQueue_Until_Queue_Empty),
};

// Read-only structure that is exposed to this file only
static const TestTemplateGroup all_template_groups[] =
{
    TEST_TEMPLATE_GROUP_ENTRY(test_task_templates),
    TEST_TEMPLATE_GROUP_ENTRY(test_node_templates),
    TEST_TEMPLATE_GROUP_ENTRY(test_queue_templates),
    TEST_TEMPLATE_GROUP_ENTRY(test_executor_templates),
};

static const size_t num_template_groups = sizeof(all_template_groups) / sizeof(all_template_groups[0]);

static size_t Get_Num_Templates()
{
    size_t num_templates = 0;
    for (size_t i = 0; i < num_template_groups; ++i)
    {
        num_templates += all_template_groups[i].size;
    }
    return num_templates;
}

static int setup_test_context(void **state)
{
    TestCase *test_case = *state;
    if (test_case != NULL && test_case->ctx != NULL)
    {
        TestCtx *ctx = test_case->ctx;
        ctx->was_called = false;
        ctx->result = 0;
    }
    return 0;
}

int main(void)
{
    const size_t num_templates = Get_Num_Templates();

    static TestCtx ctxs[] = {
        default_ctx,
    };

    static const TestCase test_cases[] = {
        { .fn = Test_Task_Fn, .ctx = &ctxs[0] },
        { .fn = Test_Task_Fn, .ctx = NULL },
        { .fn = NULL, .ctx = NULL },
    };

    const size_t num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    const size_t total_tests = num_cases * num_templates;

    printf(
        "[ ------------------------------]\n"
        "  Running %zu tests\n"
        "  Number of Cases: %zu\n"
        "  Number of Templates %zu\n",
        total_tests, num_cases, num_templates);

    // Initialize all CMUnitTests to 0
    struct CMUnitTest *tests = calloc(total_tests, sizeof(*tests));
    if (tests == NULL)
    {
        return 1;
    }

    printf(
        "  Size of Tests pointer: %zu bytes\n",
        sizeof(*tests));

    size_t test_index = 0;
    for (size_t test_case_index = 0; test_case_index < num_cases; test_case_index++) {
        for (size_t group_index = 0; group_index < num_template_groups; group_index++) {
            for (size_t template_index = 0; template_index < all_template_groups[group_index].size; template_index++)
            {
                tests[test_index++] = (struct CMUnitTest){
                    .name = all_template_groups[group_index].array[template_index].name,
                    .test_func = all_template_groups[group_index].array[template_index].test_func,
                    .setup_func = setup_test_context,
                    .teardown_func = NULL,
                    .initial_state = &test_cases[test_case_index],
                };
            }
        }
    }


    // TODO: CALCULATE TOTAL MEMORY ALLOCATIONS NEEDED FOR TESTS
    printf(
        "  Total Size of Tests struct: %zu bytes\n"
        "[ ------------------------------]\n\n",
        sizeof(*tests));

    int rc = _cmocka_run_group_tests("tasklab", tests, total_tests, NULL, NULL);
    free(tests);
    return rc;
}


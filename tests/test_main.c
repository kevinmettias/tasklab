#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

#include "test_task.h"
#include "test_node.h"
#include "test_queue.h"
#include "test_executor.h"

#define TEST_ENTRY(name) { #name, name }
#define TEST_TEMPLATE_GROUP_ENTRY(name) { name, sizeof(name)/sizeof(name[0]) }

static const int NUM_ARGUMENTS = 3;
static const int TEST_RUNNER_ARGUMENT = 0;
static const int RUN_ONE_ARGUMENT = 1;
static const int TEST_INDEX_ARGUMENT = 2;

static const int TEST_INDEX_BASE = 10;

static const int EXIT_CODE_SUCCESS = 0;
static const int EXIT_CODE_FAILURE = 1;

static const char STRING_TERMINATOR = '\0';

typedef struct {
    const char *name;
    CMUnitTestFunction test_func;
} TestTemplate;

typedef struct {
    const TestTemplate *array;
    size_t size;
} TestTemplateGroup;

static const TestCtx DEFAULT_CTX = { .input = 5 };

static const TestTemplate TEST_TASK_TEMPLATES[] = {
    TEST_ENTRY(Test_Create_And_Initialize_Task_Transitions_State_To_Created),
    TEST_ENTRY(Test_Run_Task_Transitions_State_To_Done),
};

static const TestTemplate TEST_NODE_TEMPLATES[] = {
    TEST_ENTRY(Test_Create_And_Initialize_TaskNode),
};

static const TestTemplate TEST_QUEUE_TEMPLATES[] = {
    TEST_ENTRY(Test_Create_And_Initalize_TaskQueue),
    TEST_ENTRY(Test_Push_To_End_Of_TaskQueue),
    TEST_ENTRY(Test_Create_And_Initalize_TaskQueue_And_Pop_From_Top_Of_TaskQueue),
    TEST_ENTRY(Test_Pop_Empty_TaskQueue),
};

static const TestTemplate TEST_EXECUTOR_TEMPLATES[] = {
    TEST_ENTRY(Test_Create_And_Initialize_Task_Executor),
    TEST_ENTRY(Test_Create_And_Initialize_Task_Executor_With_New_Queue),
    TEST_ENTRY(Test_Execute_Tasks_In_TaskQueue_Until_Queue_Empty),
    TEST_ENTRY(Test_Execute_Empty_TaskQueue_Is_Safe),
    TEST_ENTRY(Test_Execute_Empty_TaskQueue_Does_Not_Invoke_Failure_Handler),
    TEST_ENTRY(Test_Execute_Multiple_Tasks_In_Submission_Order),
};

// Read-only structure that is exposed to this file only
static const TestTemplateGroup ALL_TEMPLATE_GROUPS[] =
{
    TEST_TEMPLATE_GROUP_ENTRY(TEST_TASK_TEMPLATES),
    TEST_TEMPLATE_GROUP_ENTRY(TEST_NODE_TEMPLATES),
    TEST_TEMPLATE_GROUP_ENTRY(TEST_QUEUE_TEMPLATES),
    TEST_TEMPLATE_GROUP_ENTRY(TEST_EXECUTOR_TEMPLATES),
};

static const size_t NUM_TEMPLATE_GROUPS = sizeof(ALL_TEMPLATE_GROUPS) / sizeof(ALL_TEMPLATE_GROUPS[0]);

static size_t Get_Num_Templates()
{
    size_t num_templates = 0;
    for (size_t i = 0; i < NUM_TEMPLATE_GROUPS; ++i)
    {
        num_templates += ALL_TEMPLATE_GROUPS[i].size;
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

static struct CMUnitTest *Build_Tests(size_t *total_tests_out)
{
    const size_t num_templates = Get_Num_Templates();

    static TestCtx ctxs[] = {
        DEFAULT_CTX,
    };

    static TestCase test_cases[] = {
        { .fn = Test_Task_Fn, .ctx = &ctxs[0] },
        { .fn = Test_Task_Fn, .ctx = NULL },
        { .fn = NULL, .ctx = NULL },
    };

    const size_t num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    const size_t total_tests = num_cases * num_templates;

    struct CMUnitTest *tests = calloc(total_tests, sizeof(*tests));
    if (tests == NULL)
    {
        *total_tests_out = 0;
        return NULL;
    }

    size_t test_index = 0;
    for (size_t test_case_index = 0; test_case_index < num_cases; test_case_index++) {
        for (size_t group_index = 0; group_index < NUM_TEMPLATE_GROUPS; group_index++) {
            for (size_t template_index = 0; template_index < ALL_TEMPLATE_GROUPS[group_index].size; template_index++)
            {
                tests[test_index++] = (struct CMUnitTest){
                    .name = ALL_TEMPLATE_GROUPS[group_index].array[template_index].name,
                    .test_func = ALL_TEMPLATE_GROUPS[group_index].array[template_index].test_func,
                    .setup_func = setup_test_context,
                    .teardown_func = NULL,
                    .initial_state = &test_cases[test_case_index],
                };
            }
        }
    }

    *total_tests_out = total_tests;
    return tests;
}

static int Run_One_Test(size_t test_index)
{
    size_t total_tests = 0;
    struct CMUnitTest *tests = Build_Tests(&total_tests);
    if (tests == NULL)
    {
        return EXIT_CODE_FAILURE;
    }

    if (test_index >= total_tests)
    {
        free(tests);
        return EXIT_CODE_FAILURE;
    }

    struct CMUnitTest test = tests[test_index];
    int rc = _cmocka_run_group_tests("c-task-lab", &test, 1, NULL, NULL);
    free(tests);
    return rc;
}

static void Initialize_Test_Args(char *buf, size_t buf_size, size_t test_index)
{
    snprintf(buf, buf_size, "%zu", test_index);
}

static int Run_Test_In_Child(const char *runner_path, size_t test_index)
{
    int const STRING_BUFFER_SIZE = 32;
    char test_index_string_buffer[STRING_BUFFER_SIZE];
    Initialize_Test_Args(test_index_string_buffer, STRING_BUFFER_SIZE, test_index);

#ifdef _WIN32
    const char *args[] = { runner_path, "--run-one", test_index_string_buffer, NULL };
    return _spawnv(_P_WAIT, runner_path, args);
#else
    pid_t pid = fork();
    if (pid < 0)
    {
        return 1;
    }

    if (pid == 0)
    {
        execl(runner_path, runner_path, "--run-one", test_index_string_buffer, (char *)NULL);
        _exit(127);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0)
    {
        return 1;
    }

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status))
    {
        return 128 + WTERMSIG(status);
    }

    return 1;
#endif
}

static bool Is_Run_One_Argument(char **argv)
{
    return strcmp(argv[RUN_ONE_ARGUMENT], "--run-one") == 0;
}

int main(int argc, char **argv)
{
    if (argc == NUM_ARGUMENTS && Is_Run_One_Argument(argv))
    {
        char *end = NULL;
        unsigned long test_index = strtoul(argv[2], &end, TEST_INDEX_BASE);
        if (end == argv[TEST_INDEX_ARGUMENT] || *end != STRING_TERMINATOR)
        {
            return EXIT_CODE_FAILURE;
        }
        return Run_One_Test(test_index);
    }

    size_t total_tests = 0;
    struct CMUnitTest *tests = Build_Tests(&total_tests);
    if (tests == NULL)
    {
        return EXIT_CODE_FAILURE;
    }

    const size_t num_templates = Get_Num_Templates();
    const size_t num_cases = total_tests / num_templates;

    printf(
        "[ ------------------------------]\n"
        "  Running %zu tests\n"
        "  Number of Cases: %zu\n"
        "  Number of Templates %zu\n"
        "[ ------------------------------]\n\n",
        total_tests, num_cases, num_templates);
    fflush(stdout);

    size_t failed_tests = 0;
    size_t successful_tests = 0;
    for (size_t test_index = 0; test_index < total_tests; ++test_index)
    {
        int rc = Run_Test_In_Child(argv[TEST_RUNNER_ARGUMENT], test_index);
        if (rc != EXIT_CODE_SUCCESS)
        {
            failed_tests++;
            printf("[  FAILED  ] #%zu %s exited with code %d\n", test_index, tests[test_index].name, rc);
            fflush(stdout);
        }
        else
        {
            successful_tests++;
        }
    }

    printf(
        "\n[ ------------------------------]\n"
        "    Successful tests: %zu / %zu\n"
        "    Failed tests: %zu / %zu\n"
        "[ ------------------------------]\n",
        successful_tests,
        total_tests,
        failed_tests,
        total_tests);

    free(tests);
    return failed_tests == 0
        ? EXIT_CODE_SUCCESS
        : EXIT_CODE_FAILURE;
}

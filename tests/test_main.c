#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_tl_task.h"


int main(void) {
    const TestCtx default_ctx = { .input = 5 };

    static TestCtx ctxs[] = {
        default_ctx,
    };

    static TestCase test_cases[] = {
        { .fn = test_task_fn, .ctx = &ctxs[0] },
        { .fn = test_task_fn, .ctx = NULL },
        { .fn = NULL, .ctx = NULL },
    };

    const size_t num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    const size_t tests_per_case = 2;
    const size_t total_tests = num_cases * tests_per_case;

    struct CMUnitTest *tests = calloc(total_tests, sizeof(*tests));
    if (tests == NULL) {
        return 1;
    }

    size_t k = 0;
    for (size_t i = 0; i < num_cases; ++i) {
        tests[k++] = (struct CMUnitTest)
        {
            .name = "test_create_task_transition_state_to_create",
            .test_func = test_create_task_transition_state_to_create,
            .setup_func = NULL,
            .teardown_func = NULL,
            .initial_state = &test_cases[i],
        };

        tests[k++] = (struct CMUnitTest)
        {
            .name = "test_run_task_transition_state_to_done",
            .test_func = test_run_task_transition_state_to_done,
            .setup_func = NULL,
            .teardown_func = NULL,
            .initial_state = &test_cases[i],
        };
    }

    int rc = _cmocka_run_group_tests("tasklab", tests, k, NULL, NULL);
    free(tests);
    return rc;
}

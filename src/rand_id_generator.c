#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "rand_id_generator.h"

const int HALF_WORD_SIZE = 32;

uint64_t Get_uint64_t_With_Top_Half_Random()
{
    return (uint64_t)rand() << HALF_WORD_SIZE;
}

uint64_t Get_Random_uint64_t()
{
    return Get_uint64_t_With_Top_Half_Random() | rand();
}

uint64_t Random_uint64_t_Id(void)
{
    static bool seeded = false;
    if (!seeded) {
        time_t now = time(NULL); // Pass NULL pointer as to not store results anywhere in memory.
        srand((unsigned)now); // Set starting seed used by rand() to current time
        seeded = true;
    }
    return Get_Random_uint64_t();
}



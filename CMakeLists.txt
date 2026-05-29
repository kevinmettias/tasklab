cmake_minimum_required(VERSION 3.20)
project(tasklab C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(TASKLAB_ENABLE_ASAN "Enable AddressSanitizer/UBSan in Debug builds" OFF)

if (TASKLAB_ENABLE_ASAN AND CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(_tasklab_vcpkg_triplet "x64-windows")
set(_tasklab_vcpkg_roots
    "$ENV{VCPKG_ROOT}"
    "C:/dev/vcpkg"
)

foreach(_tasklab_vcpkg_root IN LISTS _tasklab_vcpkg_roots)
    if(_tasklab_vcpkg_root AND EXISTS "${_tasklab_vcpkg_root}/installed/${_tasklab_vcpkg_triplet}/share/cmocka/cmocka-config.cmake")
        set(cmocka_DIR "${_tasklab_vcpkg_root}/installed/${_tasklab_vcpkg_triplet}/share/cmocka")
        break()
    endif()
endforeach()

add_executable(tasklab
    src/main.c
    src/rand_id_generator.c
    src/task.c
    src/task_node.c
    src/task_queue.c
    src/task_executor.c
)

find_package(cmocka CONFIG REQUIRED)

add_executable(test_runner
    tests/test_main.c
    tests/test_task.c
    tests/test_node.c
    tests/test_queue.c
    tests/test_executor.c
    src/rand_id_generator.c
    src/task.c
    src/task_node.c
    src/task_queue.c
    src/task_executor.c
)

target_link_libraries(test_runner PRIVATE cmocka::cmocka)

if (MSVC)
    target_compile_options(tasklab PRIVATE /W4)
    target_compile_options(test_runner PRIVATE /W4)
else()
    target_compile_options(tasklab PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(test_runner PRIVATE -Wall -Wextra -Wpedantic)
endif()

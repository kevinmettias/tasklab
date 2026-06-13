cmake_minimum_required(VERSION 3.20)
project(tasklab C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(TASKLAB_ENABLE_ASAN "Enable AddressSanitizer instrumentation" OFF)
option(TASKLAB_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer instrumentation" OFF)
option(TASKLAB_ENABLE_LSAN "Enable LeakSanitizer instrumentation" OFF)
option(TASKLAB_ENABLE_COVERAGE "Enable coverage instrumentation" OFF)

set(_tasklab_vcpkg_triplet "x64-mingw-dynamic")
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

find_package(cmocka CONFIG QUIET)
if(cmocka_FOUND)
    if(TARGET cmocka::cmocka)
        set(CMOCKA_TARGET cmocka::cmocka)
    else()
        set(CMOCKA_TARGET cmocka)
    endif()
else()
    find_library(CMOCKA_TARGET cmocka REQUIRED)
endif()

set(TASKLAB_CORE_SOURCES
    src/rand_id_generator.c
    src/task.c
    src/task_node.c
    src/task_queue.c
    src/task_executor.c
)

add_library(tasklab_core ${TASKLAB_CORE_SOURCES})
target_include_directories(tasklab_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(tasklab src/main.c)
target_link_libraries(tasklab PRIVATE tasklab_core)

add_executable(bench_runner bench/bench_main.c)
target_link_libraries(bench_runner PRIVATE tasklab_core)

add_executable(test_runner
    tests/test_main.c
    tests/test_task.c
    tests/test_node.c
    tests/test_queue.c
    tests/test_executor.c
)

target_link_libraries(test_runner PRIVATE tasklab_core ${CMOCKA_TARGET})

if (MSVC)
    set(_tasklab_warning_flags /W4)
else()
    set(_tasklab_warning_flags -Wall -Wextra -Wpedantic)
endif()

target_compile_options(tasklab_core PRIVATE ${_tasklab_warning_flags})
target_compile_options(tasklab PRIVATE ${_tasklab_warning_flags})
target_compile_options(bench_runner PRIVATE ${_tasklab_warning_flags})
target_compile_options(test_runner PRIVATE ${_tasklab_warning_flags})

if (TASKLAB_ENABLE_COVERAGE)
    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(tasklab_core PRIVATE --coverage -O0 -g)
        target_compile_options(tasklab PRIVATE --coverage -O0 -g)
        target_compile_options(bench_runner PRIVATE --coverage -O0 -g)
        target_compile_options(test_runner PRIVATE --coverage -O0 -g)
        target_link_options(tasklab PRIVATE --coverage)
        target_link_options(bench_runner PRIVATE --coverage)
        target_link_options(test_runner PRIVATE --coverage)
    else()
        message(FATAL_ERROR "TASKLAB_ENABLE_COVERAGE requires GCC or Clang")
    endif()
endif()

if (TASKLAB_ENABLE_ASAN OR TASKLAB_ENABLE_UBSAN OR TASKLAB_ENABLE_LSAN)
    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        set(_tasklab_sanitizers)
        if (TASKLAB_ENABLE_ASAN)
            list(APPEND _tasklab_sanitizers address)
        endif()
        if (TASKLAB_ENABLE_UBSAN)
            list(APPEND _tasklab_sanitizers undefined)
        endif()
        if (TASKLAB_ENABLE_LSAN)
            list(APPEND _tasklab_sanitizers leak)
        endif()

        list(JOIN _tasklab_sanitizers "," _tasklab_sanitizer_flags)
        target_compile_options(tasklab_core PRIVATE -fsanitize=${_tasklab_sanitizer_flags} -fno-omit-frame-pointer -O1 -g)
        target_compile_options(tasklab PRIVATE -fsanitize=${_tasklab_sanitizer_flags} -fno-omit-frame-pointer -O1 -g)
        target_compile_options(bench_runner PRIVATE -fsanitize=${_tasklab_sanitizer_flags} -fno-omit-frame-pointer -O1 -g)
        target_compile_options(test_runner PRIVATE -fsanitize=${_tasklab_sanitizer_flags} -fno-omit-frame-pointer -O1 -g)
        target_link_options(tasklab PRIVATE -fsanitize=${_tasklab_sanitizer_flags})
        target_link_options(bench_runner PRIVATE -fsanitize=${_tasklab_sanitizer_flags})
        target_link_options(test_runner PRIVATE -fsanitize=${_tasklab_sanitizer_flags})
    else()
        message(FATAL_ERROR "TASKLAB sanitizers require GCC or Clang")
    endif()
endif()

enable_testing()
add_test(NAME test_runner COMMAND test_runner)


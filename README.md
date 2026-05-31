# tasklab

Task scheduling lab for learning task execution, queue behavior, and eventually work-stealing and DAG-driven scheduling.

## Project layout

- `include/` — public headers
- `src/` — implementation files
- `tests/` — cmocka test runner and module tests
- `bench/` — benchmark harnesses
- `docs/` — design notes, lab guide, architecture notes, benchmarks, and local development docs
- `scripts/` — local helper scripts for build/test/dev workflows

## Build

### CMake (recommended)

```powershell
cmake --preset debug
cmake --build --preset debug
```

### CMake + vcpkg on Windows

```powershell
$env:VCPKG_ROOT = "C:\dev\vcpkg"
cmake --preset debug-vcpkg
cmake --build --preset debug-vcpkg
```

### Makefile fallback

```powershell
make
```

## Test

### CTest

```powershell
ctest --preset debug --output-on-failure
```

### Windows + vcpkg

```powershell
$env:VCPKG_ROOT = "C:\dev\vcpkg"
ctest --preset debug-vcpkg
```

### Makefile fallback

```powershell
make test
```

## Debug tests in CLion

Use the `test_runner` target directly rather than running `make test` through a shell.

1. Open the project as a CMake project.
2. In **Settings → Build, Execution, Deployment → CMake**, choose:
   - `debug` on Linux/WSL, or
   - `debug-vcpkg` on Windows with vcpkg-managed `cmocka`
3. Reload CMake.
4. Build `test_runner`.
5. Debug `test_runner` and place breakpoints in `tests/test_main.c` or any module test/source file.

## Sanitizer policy

- Native Windows/MSYS2 GCC builds may not provide working AddressSanitizer runtime libraries.
- `TASKLAB_ENABLE_UBSAN` is available through CMake for GNU/Clang builds.
- Full ASan + UBSan validation is best done with the `sanitize-linux` preset on Linux/WSL.

## Local development

See `docs/local-dev.md` for environment setup, scripts, and recommended CLion workflow.


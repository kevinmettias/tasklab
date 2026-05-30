# Local Development

## Recommended workflow

Prefer the CMake + CTest path for day-to-day development and CLion debugging.

### Debug preset

```powershell
cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure
```

### Windows + vcpkg preset

```powershell
$env:VCPKG_ROOT = "C:\dev\vcpkg"
cmake --preset debug-vcpkg
cmake --build --preset debug-vcpkg --target test_runner
ctest --preset debug-vcpkg
```

In CLion:

- open **Settings → Build, Execution, Deployment → CMake**
- choose `debug` or `debug-vcpkg`
- reload CMake
- debug `test_runner` directly so breakpoints work in test code and implementation files

## Makefile fallback

The Makefile is still useful for quick local builds:

```powershell
make
make test
make clean
```

## Scripts

Helper scripts live under `scripts/`:

- `scripts/dev-env.ps1` — prepares a local MSYS2/vcpkg-oriented environment and can run a command inside it
- `scripts/local-ci.ps1` — runs the recommended local build + test loop

## Sanitizers

- Windows/MSYS2 GCC: UndefinedBehaviorSanitizer is more realistic than AddressSanitizer.
- Linux/WSL: use the `sanitize-linux` preset for full ASan + UBSan validation.

## Current assumptions

- MSYS2 is installed at `C:\msys64`
- vcpkg is installed at `C:\dev\vcpkg`
- `cmocka` is installed via vcpkg for Windows CMake workflows


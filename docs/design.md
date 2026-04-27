# tasklab — Design Notes

## Sanitizer Policy

- **AddressSanitizer (ASan)**: Not available on MinGW/Windows. Deferred to WSL for stress/sanitizer validation runs.
- **UndefinedBehaviorSanitizer (UBSan)**: Enabled in all debug builds via `-fsanitize=undefined`.
- Full ASan runs: `wsl make test` (once WSL toolchain is configured).

## Build Profiles

- **Debug** (default): `-fsanitize=undefined -g -O0 -Wall -Wextra`
- **Release** (future): `-O2 -DNDEBUG`

## Directory Layout

| Path | Purpose |
|---|---|
| `include/` | Public headers (tl_task.h, tl_pool.h, etc.) |
| `src/` | Implementation files |
| `tests/` | cmocka unit + stress tests |
| `bench/` | Benchmark harnesses |
| `docs/` | Design decisions and benchmark results |
| `build/` | Compiled artifacts (git-ignored) |


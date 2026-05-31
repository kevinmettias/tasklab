# tasklab Architecture

This project is a learning scaffold for two connected systems:

1. a work-stealing scheduler,
2. a DAG task graph that feeds ready work into the scheduler.

## Layer model

Task graph (`dependencies`) -> ready queue(s) -> worker pool -> execution + metrics.


## Modules

- `task.h` — task identity, function pointer, context, and state.
- `queue.h` — FIFO queue for task submission and execution order.
- `executor.h` — single-threaded executor for running tasks from a queue.

// Future modules (add only when needed):
// - graph.h — dependency tracking, cycle validation, and ready-node propagation.
// - metrics.h — counters for throughput, stealing, and idle behavior.

## Rules for the first implementation

- Keep the first version boring and correct.
- Use mutex-backed structures before any lock-free design.
- Measure contention and task granularity before optimizing.
- Treat strategy choices as experiment variables, not permanent design decisions.

## Questions this scaffold is meant to answer

- When does work stealing help?
- When does it hurt?
- How much does dependency depth limit parallelism?
- Which bottleneck dominates first: compute, cache, synchronization, or allocation?

## What stays out of scope initially

- lock-free deques
- CPU pinning
- NUMA tuning
- task priorities
- fibers or coroutines
- custom allocators



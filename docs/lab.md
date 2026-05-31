# tasklab Lab Guide

This project is a learning lab for building a C work-stealing scheduler and a DAG task graph on top of it.

The operating rule is:

1. isolate the problem,
2. rebuild the smallest useful version,
3. understand the behavior deeply,
4. feed the lesson back into the larger engine.

## What this lab is for

- Learn how task scheduling behaves under load.
- Compare queue designs, stealing strategies, and backoff policies.
- Understand how dependency graphs expose parallelism and bottlenecks.
- Build evidence for engine decisions instead of guessing.

## Core mental model

Task graph (`dependencies`) → ready queue(s) → worker pool → execution and metrics.

Treat these as two connected projects, not one blob:

- **Project 1:** work-stealing scheduler
- **Project 2:** DAG task graph that feeds ready work into the scheduler

## Lab rules

- Start with correctness, not cleverness.
- Keep the first version boring and measurable.
- Do not add lock-free structures until you can name the bottleneck.
- Every phase should end with a testable question.
- Record what changed in behavior, not just what compiled.

## Phase guide

### Phase 0 — Project skeleton

Goal: a build/test loop exists.

Learn:

- project layout
- CMake / CTest workflow
- how tests and benchmarks are separated

Done when:

- `tasklab` builds
- one trivial test runs
- debug builds are usable in CLion

### Phase 1 — Task abstraction

Goal: a task is just a function pointer plus context.

Learn:

- function-pointer based APIs
- task state transitions
- what belongs in the public task contract

Done when:

- a task can run a callback
- the callback receives its context
- task state is observable before and after execution

Avoid:

- futures
- return values
- cancellation
- heap-owned closures

### Phase 2 — Single-thread executor

Goal: prove task semantics before concurrency hides bugs.

Learn:

- queueing semantics
- deterministic execution order
- ownership boundaries

Done when:

- one task runs correctly
- many tasks run correctly
- the behavior is repeatable on one thread

### Phase 3 — Mutex-backed deque

Goal: understand the local queue before workers exist.

Learn:

- owner-side push/pop behavior
- thief-side steal behavior
- empty/full queue semantics
- wraparound behavior

Done when:

- owner operations and steal operations both behave predictably
- no task is lost or duplicated in stress tests

Avoid:

- lock-free deque designs
- dynamic resizing

### Phase 4 — Thread pool, no stealing

Goal: workers run tasks from a single queue.

Learn:

- worker lifecycle
- submission and shutdown semantics
- how contention appears before stealing exists

Done when:

- tasks survive start/stop boundaries
- pending work completes before shutdown
- large no-op batches do not lose tasks

### Phase 5 — Per-worker local queues

Goal: explore locality before stealing.

Learn:

- local vs global queue tradeoffs
- where contention moves when work is localized

Done when:

- local submissions run
- global fallback still works
- simple starvation cases are understood

### Phase 6 — Work stealing

Goal: idle workers can make forward progress by stealing.

Learn:

- victim selection strategies
- steal success rates
- how stealing changes throughput and fairness

Done when:

- one busy worker can be drained by others
- each task completes exactly once
- steal metrics are visible

### Phase 7 — Backoff policies

Goal: compare idle-worker behavior.

Learn:

- spin vs yield vs sleep
- how backoff affects throughput, latency, and CPU use

Done when:

- you can explain which policy helps short tasks
- you can explain which policy wastes CPU when idle

### Phase 8 — Benchmark matrix v1

Goal: classify the first real bottleneck.

Measure:

- throughput
- latency
- scaling with worker count
- contention under load
- steal success rate

Record:

- compute-bound vs cache-bound vs synchronization-bound vs allocation-bound

### Phase 9 — DAG graph, single-threaded

Goal: make dependency semantics work before parallelism.

Learn:

- dependency counters
- acyclic validation
- ready-task discovery
- topological execution order

Done when:

- simple chains work
- fan-out and fan-in work
- cycles are rejected

### Phase 10 — DAG feeds the pool

Goal: the graph submits ready tasks into the scheduler.

Learn:

- completion propagation
- how dependency depth limits parallelism
- how wide graphs expose concurrency

Done when:

- parents finish before children
- completed counts match node counts
- stress graphs behave predictably

### Phase 11 — Deterministic mode

Goal: keep a reference execution path for debugging.

Learn:

- why reproducibility matters
- how a reference mode helps compare against parallel runs

### Phase 12 — Benchmark matrix v2

Goal: measure graph shape, not just task count.

Measure:

- deep chains
- wide fan-out
- wide fan-in
- random DAGs
- uneven task costs

### Phase 13 — Strategy comparisons

Goal: compare one policy at a time.

Candidate strategy surfaces:

- victim selection
- steal amount
- queue priority
- backoff policy

Rule:

- change one surface, rerun the same tests, then compare metrics

### Phase 14 — Failure and lifecycle hardening

Goal: learn the ugly cases.

Study:

- shutdown while work is active
- nested task submission
- repeated shutdown / destroy paths
- wait semantics while submissions continue

### Phase 15 — Feedback report

Goal: turn the lab into engine insight.

Write down:

- best strategy and worst strategy
- dominant bottleneck
- minimum useful task size
- queue contention behavior
- stealing success patterns
- graph shapes that scale and shapes that do not

## What to postpone

Do not add these until the simple version is working and measured:

- lock-free Chase-Lev queues
- CPU pinning
- NUMA awareness
- task priorities
- fibers or coroutines
- custom allocators
- I/O integration

## Evidence to keep

For each phase, keep a short note answering:

- What changed?
- What failed?
- What became measurable?
- What did the measurements suggest?

That record is the actual purpose of the lab.
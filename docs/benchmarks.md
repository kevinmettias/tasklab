# tasklab Benchmark Scaffold

This document defines the first benchmark matrix for the scheduler lab.

## Metrics to capture

- throughput: tasks per second
- latency: task completion time
- scaling: behavior as worker count changes
- contention: queue and shutdown pressure
- steal success rate: attempts vs successful steals
- idle cost: CPU spent while waiting for work

## Baseline scenarios

1. no-op task throughput
2. fixed-cost task throughput
3. uneven task workload
4. all tasks submitted to one worker
5. global queue only vs local queue + stealing
6. spin vs yield vs sleep backoff
7. 1, 2, 4, 8, 16 workers

## DAG scenarios

1. deep chain
2. wide fan-out
3. wide fan-in
4. diamond graph
5. random DAG
6. uneven task-cost DAG

## Classification rule

Every benchmark run should end with a bottleneck label:

- compute-bound
- cache-bound
- synchronization-bound
- allocation-bound
- idle/backoff-bound

The point is not just to record numbers; it is to explain why the numbers look that way.


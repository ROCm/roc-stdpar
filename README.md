# ROCm Standard Parallelism Runtime Implementation

This repository contains the runtime component that enables ROCm to support
GPU accelerated computation via C++ Standard Algorithms. This is an
implementation detail and thus intended to be transparently composed with
compiler and low-level runtime components. More specifically, since it is meant
to be implicitly included by the compiler, and forwards various calls to the
HIP and rocThrust libraries, its standalone uses are limited.

## Algorithm Support Status

| Algorithm | Status   | Observations |
|:---------:|:--------:|:------------:|
| `TODO`    | **TODO** | *TODO*       |

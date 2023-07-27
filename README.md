# ROCm Standard Parallelism Runtime Implementation

This repository contains:

1. The runtime component that enables ROCm to support GPU accelerated
   computation via C++ Standard Algorithms. This is an implementation detail and
   thus intended to be transparently composed with compiler and low-level
   runtime components. More specifically, since it is meant to be implicitly
   included by the compiler, and forwards various calls to the HIP and rocThrust
   libraries, its standalone uses are limited. For detailed documentation on
   ROCm GPU acceleration for Standard C++ Algorithms, please see the
   [RFC](https://discourse.llvm.org/t/rfc-adding-c-parallel-algorithm-offload-support-to-clang-llvm/72159)
   and [the patch set](https://reviews.llvm.org/D155769).
2. A set of diffs, under the [data folder](/data/patches/), which capture the
   changes needed to enable Standard Algorithm offload in LLVM (this is a
   pre-requisite), as well as in a number of client apps;
3. A slowly growing set of minimalistic performance studies, under the
   [docs](/docs/performance_studies/) folder, intended to give a sense of the
   possible performance benefits / pitfalls associated with offload;
4. A minimal but slowly growing set of unit tests.

## Algorithm Support Status

| Algorithm                           | Status      | Observations             |
|:------------------------------------|:-----------:|:------------------------:|
| `adjacent_difference`               | SUPPORTED   |                          |
| `adjacent_find`                     | SUPPORTED   | Uses `mismatch`          |
| `all_of`                            | SUPPORTED   |                          |
| `any_of`                            | SUPPORTED   |                          |
| `copy`                              | SUPPORTED   |                          |
| `copy_if`                           | SUPPORTED   |                          |
| `copy_n`                            | SUPPORTED   |                          |
| `count`                             | SUPPORTED   |                          |
| `count_if`                          | SUPPORTED   |                          |
| `destroy`                           | SUPPORTED   |                          |
| `destroy_n`                         | SUPPORTED   |                          |
| `equal`                             | SUPPORTED   |                          |
| `exclusive_scan`                    | SUPPORTED   |                          |
| `fill`                              | SUPPORTED   |                          |
| `fill_n`                            | SUPPORTED   |                          |
| `find`                              | SUPPORTED   |                          |
| `find_end`                          | UNSUPPORTED | Missing from `rocThrust` |
| `find_first_of`                     | UNSUPPORTED | Missing from `rocThrust` |
| `find_if`                           | SUPPORTED   |                          |
| `find_if_not`                       | SUPPORTED   |                          |
| `for_each`                          | SUPPORTED   |                          |
| `for_each_n`                        | SUPPORTED   |                          |
| `generate`                          | SUPPORTED   |                          |
| `generate_n`                        | SUPPORTED   |                          |
| `includes`                          | SUPPORTED   |                          |
| `inclusive_scan`                    | SUPPORTED   |                          |
| `inplace_merge`                     | UNSUPPORTED | Missing from `rocThrust` |
| `is_heap`                           | UNSUPPORTED | Missing from `rocThrust` |
| `is_heap_until`                     | UNSUPPORTED | Missing from `rocThrust` |
| `is_partitioned`                    | SUPPORTED   |                          |
| `is_sorted`                         | SUPPORTED   |                          |
| `is_sorted_until`                   | SUPPORTED   |                          |
| `lexicographical_compare`           | SUPPORTED   | Uses `mismatch`          |
| `max_element`                       | SUPPORTED   |                          |
| `merge`                             | SUPPORTED   |                          |
| `min_element`                       | SUPPORTED   |                          |
| `minmax_element`                    | SUPPORTED   |                          |
| `mismatch`                          | SUPPORTED   |                          |
| `move`                              | SUPPORTED   |                          |
| `none_of`                           | SUPPORTED   |                          |
| `nth_element`                       | UNSUPPORTED | Missing from `rocThrust` |
| `partial_sort`                      | UNSUPPORTED | Missing from `rocThrust` |
| `partial_sort_copy`                 | UNSUPPORTED | Missing from `rocThrust` |
| `partition`                         | SUPPORTED   |                          |
| `partition_copy`                    | SUPPORTED   |                          |
| `reduce`                            | SUPPORTED   |                          |
| `remove`                            | SUPPORTED   |                          |
| `remove_copy`                       | SUPPORTED   |                          |
| `remove_copy_if`                    | SUPPORTED   |                          |
| `remove_if`                         | SUPPORTED   |                          |
| `replace`                           | SUPPORTED   |                          |
| `replace_copy`                      | SUPPORTED   |                          |
| `replace_copy_if`                   | SUPPORTED   |                          |
| `replace_if`                        | SUPPORTED   |                          |
| `reverse`                           | SUPPORTED   |                          |
| `reverse_copy`                      | SUPPORTED   |                          |
| `rotate`                            | UNSUPPORTED | Missing from `rocThrust` |
| `rotate_copy`                       | UNSUPPORTED | Missing from `rocThrust` |
| `search`                            | UNSUPPORTED | Missing from `rocThrust` |
| `search_n`                          | UNSUPPORTED | Missing from `rocThrust` |
| `set_difference`                    | SUPPORTED   |                          |
| `set_intersection`                  | SUPPORTED   |                          |
| `set_symmetric_difference`          | SUPPORTED   |                          |
| `set_union`                         | SUPPORTED   |                          |
| `shift_left`                        | UNSUPPORTED | Missing from `rocThrust` |
| `shift_right`                       | UNSUPPORTED | Missing from `rocThrust` |
| `sort`                              | SUPPORTED   |                          |
| `stable_partition`                  | SUPPORTED   |                          |
| `stable_sort`                       | SUPPORTED   |                          |
| `swap_ranges`                       | SUPPORTED   |                          |
| `transform`                         | SUPPORTED   |                          |
| `transform_exclusive_scan`          | SUPPORTED   |                          |
| `transform_inclusive_scan`          | SUPPORTED   |                          |
| `transform_reduce`                  | SUPPORTED   |                          |
| `uninitialized_copy`                | SUPPORTED   |                          |
| `uninitialized_copy_n`              | SUPPORTED   |                          |
| `uninitialized_default_construct`   | SUPPORTED   |                          |
| `uninitialized_default_construct_n` | SUPPORTED   |                          |
| `uninitialized_fill`                | SUPPORTED   |                          |
| `uninitialized_fill_n`              | SUPPORTED   |                          |
| `unitialized_move`                  | SUPPORTED   |                          |
| `unitialized_move_n`                | SUPPORTED   |                          |
| `uninitialized_value_construct`     | SUPPORTED   |                          |
| `uninitialized_value_construct_n`   | SUPPORTED   |                          |
| `unique`                            | SUPPORTED   |                          |
| `unique_copy`                       | SUPPORTED   |                          |

## Allocation / Deallocation Interposition Status

| Function                                                        | Status    |
|:----------------------------------------------------------------|:---------:|
| `aligned_alloc`                                                 | SUPPORTED |
| `calloc`                                                        | SUPPORTED |
| `free`                                                          | SUPPORTED |
| `malloc`                                                        | SUPPORTED |
| `memalign`                                                      | SUPPORTED |
| `posix_memalign`                                                | SUPPORTED |
| `realloc`                                                       | SUPPORTED |
| `realloc_array`                                                 | SUPPORTED |
| `operator delete[](void*)`                                      | SUPPORTED |
| `operator delete[](void*, std::size_t)`                         | SUPPORTED |
| `operator delete[](void*, std::align_val_t)`                    | SUPPORTED |
| `operator delete[](void*, std::size_t, std::align_val_t)`       | SUPPORTED |
| `operator delete(void*)`                                        | SUPPORTED |
| `operator delete(void*, std::size_t)`                           | SUPPORTED |
| `operator delete(void*, std::align_val_t)`                      | SUPPORTED |
| `operator delete(void*, std::size_t, std::align_val_t)`         | SUPPORTED |
| `operator new[](std::size_t)`                                   | SUPPORTED |
| `operator new[](std::size_t, std::nothrow_t)`                   | SUPPORTED |
| `operator new[](std::size_t, std::align_val_t)`                 | SUPPORTED |
| `operator new[](std::size_t, std::align_val_t, std::nothrow_t)` | SUPPORTED |
| `operator new(std::size_t)`                                     | SUPPORTED |
| `operator new(std::size_t, std::nothrow_t)`                     | SUPPORTED |
| `operator new(std::size_t, std::align_val_t)`                   | SUPPORTED |
| `operator new(std::size_t, std::align_val_t, std::nothrow_t)`   | SUPPORTED |
| `__builtin_calloc`                                              | SUPPORTED |
| `__builtin_free`                                                | SUPPORTED |
| `__builtin_malloc`                                              | SUPPORTED |
| `__builtin_operator_delete`                                     | SUPPORTED |
| `__builtin_operator_new`                                        | SUPPORTED |
| `__builtin_realloc`                                             | SUPPORTED |
| `__libc_calloc`                                                 | SUPPORTED |
| `__libc_free`                                                   | SUPPORTED |
| `__libc_malloc`                                                 | SUPPORTED |
| `__libc_memalign`                                               | SUPPORTED |
| `__libc_realloc`                                                | SUPPORTED |

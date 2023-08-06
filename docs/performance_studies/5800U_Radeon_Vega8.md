# Performance Study: AMD Ryzen 7 5800U && AMD Radeon Vega 8

## Testing Conventions

All measurements are taken via each application's built-in tests. No
modifications to the testing protocols are done. We report data as is, retaining
units; please consult each application's documentation for details on their
benchmarks. No attempts are made to optimise any of the applications, they are
used as is. No assessments are made about the applications' optimality, be it
for the CPU or for the GPU.

For all tables, the final column contains the GPU's positioning relative to the
CPU:

- for benchmarks that report throughput (e.g. GFLOPS, MLUPS etc.), where a
  greater value is preferable, GPU throughput is divided by CPU throughput;
- for benchmarks that report time-to-solution (e.g. s, ms etc.), where a lesser
  value is preferable, CPU time is divided by GPU time.

Values less than 1 in this column indicate a CPU speed advantage. Source code
is the same for both the CPU and the GPU. Unless otherwise stated, all tests are
run in HMM mode i.e. they rely on on-demand paging and use OS allocations
directly.

## Configuration

- CPU: [AMD Ryzen 7 5800U](https://www.techpowerup.com/cpu-specs/ryzen-7-5800u.c2746)
- GPU: [AMD Radeon Vega 8](https://www.techpowerup.com/gpu-specs/radeon-graphics-512sp-mobile.c3771)
- Platform: [Lenovo ThinkBook 13s Gen3](https://www.lenovo.com/gb/en/p/laptops/thinkbook/thinkbooks/thinkbook-13s-g3-acn/xxtbxtsa301#tech_specs)
- RAM: 16GiB (2GiB GFX Aperture) LPDDR4 4266MHz 54.6GB/s
- OS: [Ubuntu 23.04](https://discourse.ubuntu.com/t/lunar-lobster-release-notes/31910)
  - Kernel version: `6.5.0-060500rc3-generic`
- Compiler: Clang/LLVM `main` latest +
  [patch](../../data/patches/LLVM/clang_llvm.patch)
- ROCm Components:
  - `rocm-hip-runtime-dev5.6.0`: 5.6.0.50600-67~22.04
  - `rocthrust-dev5.6.0`: 2.18.0.50600-67~22.04
  - `rocprim-dev5.6.0`: 2.13.0.50600-67~22.04
- Standard Library:
  - `libstdc++-12-dev`: 12.3.0-1ubuntu1~23.04
- TBB: 2021.8.0-1ubuntu2

## BabelStream

- [repo](https://github.com/UoB-HPC/BabelStream)
- [patch](../../data/patches/BabelStream/BABELSTREAM.patch)

| Test  | A - CPU (GB/s) | B - GPU (GB/s) | B / A (higher is better) |
|:-----:|:--------------:|:--------------:|:------------------------:|
| Copy  | 15             | 35             | 2                        |
| Mul   | 15             | 34             | 2                        |
| Add   | 17             | 36             | 2                        |
| Triad | 17             | 36             | 2                        |
| Dot   | 26             | 45             | 1.7                      |

## `cloverleaf_stdpar`

- [repo](https://github.com/UoB-HPC/cloverleaf_stdpar)
- [patch](../../data/patches/cloverleaf_stdpar/CLOVERLEAF_STDPAR.patch)

| Test             | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:----------------:|:-----------:|:-----------:|:------------------------:|
| clover_bm2_short | 15          | 10          | 1.5                      |
| clover_bm4_short | 30          | 18          | 1.6                      |
| clover_bm8_short | 56          | 37          | 1.5                      |
| clover_qa        | 8           | 5           | 1.6                      |
| clover_sodbig    | 10          | 7           | 1.4                      |

## CppCon19-stdpar

- [repo](https://github.com/dkolsen-pgi/CppCon19-stdpar)
- [patch](../../data/patches/CppCon19-stdpar/CPPCON19_STDPAR.patch)

| Test    | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:-------:|:-----------:|:-----------:|:------------------------:|
| jacobi1 | 1           | 2           | 0.5                      |
| jacobi2 | 1           | 2           | 0.5                      |

## `cuda-stdpar-samples`

- [repo](https://github.com/heidrifx/cuda-stdpar-samples)
- [patch](../../data/patches/cuda-stdpar-samples/)

| Test                      | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:-------------------------:|:------------:|:------------:|:------------------------:|
| daxpy                     | 146          | 4926         | 0.02                     |
| filterMapReduce           | 248          | 2448         | 0.1                      |
| filterMapReduce_optimized | 40           | 31420        | 0.001                    |
| mandelbrot                | 185452       | 18402        | 10                       |
| saxpy                     | 145          | 4728         | 0.03                     |
| sgemm                     | 8667         | 26237        | 0.33                     |
| vectorAdd                 | 96           | 3299         | 0.02                     |
| vectorReduce              | 420          | 3037         | 0.13                     |

- notes:
  - run as `./aggregateData_local.sh 3 3 1`
  - results are the average of the 3 runs

## `himeno-stdpar`

- [repo](https://github.com/asix001/himeno-stdpar)
- [patch](../../data/patches/himeno-stdpar/HIMENO.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| SMALL  | 11               | 13               | 1.1                      |
| MIDDLE | 8                | 13               | 1.6                      |
| LARGE  | 7                | 11               | 1.5                      |

## miniBUDE

- [repo](https://github.com/UoB-HPC/miniBUDE)
- [patch](../../data/patches/miniBUDE/MINIBUDE.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| bm1    | 31               | 294              | 9                        |
| bm2    | 26               | 217              | 8                        |

## `nbody-stdpar`

- [repo](https://github.com/jbeisman/nbody-stdpar)
- [patch](../../data/patches/nbody-stdpar/NBODY_STDPAR.patch)

| Test         | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:------------:|:------------:|:------------:|:------------------------:|
| 4096 bodies  | 11235        | 3036         | 3.7                      |
| 8192 bodies  | 39324        | 5921         | 6.6                      |
| 16384 bodies | 151056       | 21394        | 7                        |
| 32768 bodies | 604734       | 63774        | 9.4                      |
| 65536 bodies | 2344380      | 317180       | 7.3                      |

- notes:
  - run parameterised with `Simulation duration == 100` and
    `Integration timestep == 0.1`

## p2r-tests

- [repo](https://github.com/cerati/p2r-tests)
- [patch](../../data/patches/p2r-tests/P2R_TESTS.patch)

| Test       | A - CPU (s)  | B - GPU (s)  | A / B (higher is better) |
|:----------:|:------------:|:------------:|:------------------------:|
| 8192 trks  | 5            | 4            | 1.25                     |
| 16384 trks | 10           | 12           | 0.83                     |
| 32768 trks | 21           | 45           | 0.46                     |

- notes:
  - table records total time i.e. `setup time` + `tot time`
  - `bsize = 64`
  - other parameters kept at their default values:
    - `nevts = 100`
    - `smearing = 0.0001`
    - `NITER = 5`
    - `nlayer = 20`

## Palabos

- [repo](https://gitlab.com/unigehpfs/palabos)
- [patch](../../data/patches/palabos/PALABOS.patch)

| Test             | A - CPU (MLUPS) | B - GPU (MLUPS) | B / A (higher is better) |
|:----------------:|:---------------:|:---------------:|:------------------------:|
| rayleighTaylor3D | 8               | 0.63            | 0.07                     |
| sandstone        | 73              | 73              | 1                        |

## `paralg`

- [repo](https://gitlab.com/unigehpfs/paralg)
- [patch](../../data/patches/paralg/PARALG.patch)

| Test           | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:--------------:|:----------------:|:----------------:|:------------------------:|
| paralg_column  | 9                | 0.78             | 0.01                     |
| paralg_element | 9                | 7                | 9                        |

## STLBM - Lattice Boltzmann simulation framework based on C++ Parallel Algorithms

- [repo](https://gitlab.com/unigehpfs/stlbm)
- [patch](../../data/patches/stlbm/STLBM.patch)

| Test             | A - CPU (MLUPS) | B - GPU (MLUPS) | B / A (higher is better) |
|:----------------:|:---------------:|:---------------:|:------------------------:|
| twopop_aos       | 21              | 14              | 0.6                      |
| twopop_soa       | 31              | 65              | 2                        |
| swap_aos         | 23              | 6               | 0.2                      |
| swap_soa         | 28              | 32              | 1.1                      |
| aa_aos           | 37              | 16              | 0.4                      |
| aa_soa           | 43              | 59              | 1.3                      |

## `traccc`

- [repo](https://github.com/ugGit/traccc)
- [patch](../../data/patches/traccc/TRACCC.patch)

| Test                               | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:----------------------------------:|:------------:|:------------:|:------------------------:|
| cca_stdpar_simplified_sv           | 80           | 6.6          | 12                       |
| cca_stdpar_fast_sv_1               | 85           | 6.8          | 14                       |
| cca_stdpar_fast_sv_2               | 88           | 6.8          | 13                       |
| cca_stdpar_fast_sv_2_partition_128 | 33           | 6.8          | 5                        |
| cca_stdpar_fast_sv_2_partition_256 | 40           | 6.8          | 6                        |
| cca_stdpar_fast_sv_2_partition_512 | 55           | 6.8          | 8                        |

- notes:
  - selected `tml_full/ttbar_mu300/` test case results

## `tsp`

- [repo](https://github.com/pkestene/tsp)
- [patch](../../data/patches/tsp/TSP.patch)

| Test      | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------:|:-----------:|:-----------:|:------------------------:|
| 10 cities | 0.008       | 0.15        | 0.05                     |
| 11 cities | 0.11        | 0.19        | 0.57                     |
| 12 cities | 1           | 0.66        | 1.5                      |
| 13 cities | 16          | 20          | 0.8                      |
| 14 cities | 284         | 313         | 0.9                      |

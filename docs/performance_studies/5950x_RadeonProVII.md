# Performance Study: AMD Ryzen 9 5950X && AMD Radeon Pro VII

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

- CPU: [AMD Ryzen 9 5950X](https://www.techpowerup.com/cpu-specs/ryzen-7-5800u.c2746)
- GPU: [AMD Radeon Pro VII](https://www.techpowerup.com/gpu-specs/radeon-pro-vii.c3575)
- Motherboard: [MSI X570 GAMING PLUS](https://www.msi.com/Motherboard/MPG-X570-GAMING-PLUS/Specification)
- RAM: 128GiB DDR4 3200MHz 51.2GB/s
- OS: [Ubuntu 23.10](https://discourse.ubuntu.com/t/mantic-minotaur-release-notes/35534)
  - Kernel version: `6.6.0-060600rc1-generic`
- Compiler: Clang/LLVM `main` latest +
  [patch](../../data/patches/LLVM/clang_llvm.patch)
- ROCm Components:
  - `rocm-hip-runtime-dev5.6.1`: 5.6.1.50601-93~22.04
  - `rocthrust-dev5.6.1`: 2.18.0.50601-93~22.04
  - `rocprim-dev5.6.1`: 2.13.0.50601-93~22.04
- Standard Library:
  - `libstdc++-13-dev`: 13.2.0-3ubuntu1
- TBB: 2021.9.0-2ubuntu1

## BabelStream

- [repo](https://github.com/UoB-HPC/BabelStream)
- [patch](../../data/patches/BabelStream/BABELSTREAM.patch)

| Test  | A - CPU (GB/s) | B - GPU (GB/s) | B / A (higher is better) |
|:-----:|:--------------:|:--------------:|:------------------------:|
| Copy  | 25             | 505            | 20                       |
| Mul   | 24             | 505            | 21                       |
| Add   | 27             | 547            | 20                       |
| Triad | 27             | 554            | 20                       |
| Dot   | 39             | 727            | 18                       |

## `cloverleaf_stdpar`

- [repo](https://github.com/UoB-HPC/cloverleaf_stdpar)
- [patch](../../data/patches/cloverleaf_stdpar/CLOVERLEAF_STDPAR.patch)

| Test              | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:-----------------:|:-----------:|:-----------:|:------------------------:|
| clover_bm16_short | 67          | 4           | 16                       |
| clover_bm32_short | 131         | 8           | 16                       |
| clover_bm64_short | 259         | 17          | 15                       |
| clover_qa         | 4           | 0.4         | 10                       |
| clover_sodbig     | 4           | 2           | 2                        |

## CppCon19-stdpar

- [repo](https://github.com/dkolsen-pgi/CppCon19-stdpar)
- [patch](../../data/patches/CppCon19-stdpar/CPPCON19_STDPAR.patch)

| Test    | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:-------:|:-----------:|:-----------:|:------------------------:|
| jacobi1 | 0.69        | 0.25        | 2.5                      |
| jacobi2 | 0.63        | 0.36        | 1.7                      |

## `cuda_stdpar`

- [repo](https://github.com/quxflux/cuda_stdpar)
- [patch](../../data/patches/cuda_stdpar/CUDA_STDPAR.patch)

| Test            | A - CPU (MItems/s) | B - GPU (MItems/s) | B / A (higher is better) |
|:---------------:|:------------------:|:------------------:|:------------------------:|
| AoS - 9         | 8                  | 0.4                | 0.05                     |
| AoS - 99        | 17                 | 4                  | 0.23                     |
| AoS - 1000      | 77                 | 45                 | 0.58                     |
| AoS - 9999      | 235                | 324                | 1.37                     |
| AoS - 100000    | 377                | 1322               | 3                        |
| AoS - 999999    | 41                 | 23153              | 564                      |
| AoS - 9999999   | 466                | 46388              | 99                       |
| SoA - 9         | 18                 | 0.4                | 0.02                     |
| SoA - 99        | 27                 | 4                  | 0.14                     |
| SoA - 1000      | 28                 | 44                 | 1.5                      |
| SoA - 9999      | 22                 | 313                | 14                       |
| SoA - 100000    | 22                 | 1276               | 58                       |
| SoA - 999999    | 22                 | 2034               | 92                       |
| SoA - 9999999   | 23                 | 44758              | 1946                     |

## `cuda-stdpar-samples`

- [repo](https://github.com/heidrifx/cuda-stdpar-samples)
- [patch](../../data/patches/cuda-stdpar-samples/CUDA_STDPAR_SAMPLES.patch)

| Test                      | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:-------------------------:|:------------:|:------------:|:------------------------:|
| daxpy                     | 91           | 1670         | 0.05                     |
| filterMapReduce           | 151          | 876          | 0.17                     |
| filterMapReduce_optimized | 27           | 895          | 0.03                     |
| mandelbrot                | 61816        | 2523         | 24                       |
| saxpy                     | 91           | 1565         | 0.05                     |
| sgemm                     | 8576         | 22783        | 0.37                     |
| vectorAdd                 | 59           | 1175         | 0.05                     |
| vectorReduce              | 552          | 1458         | 0.37                     |

- notes:
  - run as `./aggregateData_local.sh 3 3 1`
  - results are the average of the 3 runs

## `himeno-stdpar`

- [repo](https://github.com/asix001/himeno-stdpar)
- [patch](../../data/patches/himeno-stdpar/HIMENO.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| SMALL  | 35               | 159              | 3                        |
| MIDDLE | 16               | 193              | 11                       |
| LARGE  | 15               | 175              | 11                       |

## LULESH

- [repo](https://github.com/LLNL/LULESH)
- [patch](../../data/patches/LULESH/LULESH.patch)

| Test               | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:------------------:|:-----------:|:-----------:|:------------------------:|
| Problem size = 30  | 10          | 31          | 0.32                     |
| Problem size = 50  | 38          | 148         | 0.25                     |
| Problem size = 80  | 182         | 901         | 0.2                      |
| Problem size = 100 | 425         | 2111        | 0.2                      |

## miniBUDE

- [repo](https://github.com/UoB-HPC/miniBUDE)
- [patch](../../data/patches/miniBUDE/MINIBUDE.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| bm1    | 92               | 2007             | 21                       |
| bm2    | 68               | 1523             | 22                       |

## miniWeather

- [repo](https://github.com/jefflarkin/miniWeather)
- [patch](../../data/patches/miniWeather/MINIWEATHER.patch)

| Test            | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------------:|:-----------:|:-----------:|:------------------------:|
| THERMAL         | 18          | 4           | 4                        |
| COLLISION       | 18          | 4           | 4                        |
| MOUNTAIN        | 18          | 5           | 3                        |
| DENSITY_CURRENT | 18          | 4           | 4                        |
| INJECTION       | 19          | 4           | 4                        |

- notes:
  - run parameterised with `NX == 400`, `NZ == 200`, `OUT_FREQ == 10` and
    `SIM_TIME == 1000` as specified in the documentation

## `nbody-stdpar`

- [repo](https://github.com/jbeisman/nbody-stdpar)
- [patch](../../data/patches/nbody-stdpar/NBODY_STDPAR.patch)

| Test         | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:------------:|:------------:|:------------:|:------------------------:|
| 4096 bodies  | 2285         | 3094         | 0.7                      |
| 8192 bodies  | 8696         | 6378         | 1.3                      |
| 16384 bodies | 35638        | 12563        | 2.8                      |
| 32768 bodies | 143911       | 39085        | 3.6                      |
| 65536 bodies | 604460       | 88118        | 6.8                      |

- notes:
  - run parameterised with `Simulation duration == 100` and
    `Integration timestep == 0.1`

## p2r-tests

- [repo](https://github.com/cerati/p2r-tests)
- [patch](../../data/patches/p2r-tests/P2R_TESTS.patch)

| Test       | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:----------:|:-----------:|:-----------:|:------------------------:|
| 8192 trks  | 3           | 3           | 1                        |
| 16384 trks | 7           | 7           | 1                        |
| 32768 trks | 14          | 15          | 1.16                     |
| 65536 trks | 27          | 30          | 1                        |

- notes:
  - table records total time i.e. `setup time` + `tot time`
  - `bsize = 64`
  - other parameters kept at their default values:
    - `nevts = 100`
    - `smearing = 0.0001`
    - `NITER = 5`
    - `nlayer = 20`

## P3-miniapps

- [repo](https://github.com/yasahi-hpc/P3-miniapps)
- [patch](../../data/patches/P3-miniapps/P3_MINIAPPS.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| heat3d | 8                | 110              | 13                       |

| Test   | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:------:|:-----------:|:-----------:|:------------------------:|
| vlp4d  | 258         | 20          | 12                       |

- notes:
  - CPU testing was done using GCC13.1, due to the OpenMP requirements
  - tests run with the configurations suggested in the
    [readme](https://github.com/yasahi-hpc/P3-miniapps#run)

## Palabos

- [repo](https://gitlab.com/unigehpfs/palabos)
- [patch](../../data/patches/palabos/PALABOS.patch)

| Test             | A - CPU (MLUPS) | B - GPU (MLUPS) | B / A (higher is better) |
|:----------------:|:---------------:|:---------------:|:------------------------:|
| rayleighTaylor3D | 14              | 97              | 6                        |
| sandstone        | 62              | 599             | 9                        |
| tgv3d            | 107             | 1670            | 15                       |

## `paralg`

- [repo](https://gitlab.com/unigehpfs/paralg)
- [patch](../../data/patches/paralg/PARALG.patch)

| Test           | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:--------------:|:----------------:|:----------------:|:------------------------:|
| paralg_column  | 14               | 0.6              | 0.04                     |
| paralg_element | 14               | 131              | 9                        |

## parSTL

- [repo](https://github.com/weilewei/parSTL)
- [patch](../../data/patches/parSTL/PARSTL.patch)

| Test             | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:----------------:|:------------:|:------------:|:------------------------:|
| sort      - 10^5 | 0.1          | 2            | 0.05                     |
| sort      - 10^6 | 1            | 11           | 0.09                     |
| sort      - 10^7 | 17           | 15           | 1.1                      |
| sort      - 10^8 | 211          | 155          | 1.3                      |
| transform - 10^5 | 0.09         | 31           | 0.002                    |
| transform - 10^6 | 9            | 46           | 0.1                      |
| transform - 10^7 | 77           | 43           | 1.7                      |
| transform - 10^8 | 817          | 154          | 5.3                      |

## pp-EnKF

- [repo](https://github.com/yasahi-hpc/pp-EnKF)
- [patch](../../data/patches/pp-EnKF/PP_ENKF.patch)

| Test                 | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:--------------------:|:-----------:|:-----------:|:------------------------:|
| heat3d               | 144         | 12          | 12                       |
| lbm2d-letkf nature   | 147         | 53          | 2.7                      |
| lbm2d-letkf no_da    | 16          | 0.4         | 40                       |
| lbm2d-letkf nudging  | 116         | 29          | 4                        |
| vlp4d SLD10_large    | 271         | 18          | 15                       |

- notes:
  - GPU testing re-uses the OpenMP path except for FFTs, where hipFFT is used
  - for the CPU tests uses of `std::views::iota(n).begin()` are replaced with
    equivalent uses of `thrust::counting_iterator`; this is because the former's
    iterator type models `std::random_access_iterator` without nesting the
    associated iterator_category tag, which is required for parallelisation and
    vectorisation with the current implementation of the standard library

## STLBM - Lattice Boltzmann simulation framework based on C++ Parallel Algorithms

- [repo](https://gitlab.com/unigehpfs/stlbm)
- [patch](../../data/patches/stlbm/STLBM.patch)

| Test             | A - CPU (MLUPS) | B - GPU (MLUPS) | B / A (higher is better) |
|:----------------:|:---------------:|:---------------:|:------------------------:|
| twopop_aos       | 40              | 266             | 6                        |
| twopop_soa       | 74              | 783             | 10                       |
| swap_aos         | 44              | 113             | 3                        |
| swap_soa         | 62              | 564             | 8                        |
| aa_aos           | 71              | 270             | 3                        |
| aa_soa           | 110             | 661             | 5                        |

## TeaLeaf

- [repo](https://github.com/UoB-HPC/TeaLeaf)
- [patch](../../data/patches/TeaLeaf/TEALEAF.patch)

| Test            | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------------:|:-----------:|:-----------:|:------------------------:|
| tea_bm_4        | 14          | 3.5         | 4                        |
| tea_bm_5        | 2157        | 138         | 15                       |
| tea_bm_5e_1     | 15          | 3.5         | 4                        |
| tea_bm_5e_2     | 229         | 22          | 10                       |
| tea_bm_5e_4     | 2100        | 132         | 16                       |
| tea_bm_5e_8     | TOO LONG    | 1186        | *                        |
| tea_bm_6        | TOO LONG    | 1186        | *                        |

## `tsp`

- [repo](https://github.com/pkestene/tsp)
- [patch](../../data/patches/tsp/TSP.patch)

| Test      | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------:|:-----------:|:-----------:|:------------------------:|
| 10 cities | 0.006       | 0.16        | 0.03                     |
| 11 cities | 0.027       | 0.19        | 0.14                     |
| 12 cities | 0.25        | 0.35        | 0.7                      |
| 13 cities | 4           | 3           | 1.3                      |
| 14 cities | 124         | 44          | 2.8                      |

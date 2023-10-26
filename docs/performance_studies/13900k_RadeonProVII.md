# Performance Study: Intel Core I9-13900KF && AMD Radeon Pro VII

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

- CPU: [Intel Core I9-13900KF](https://www.techpowerup.com/cpu-specs/core-i9-13900kf.c2916)
- GPU: [AMD Radeon Pro VII](https://www.techpowerup.com/gpu-specs/radeon-pro-vii.c3575)
- Motherboard: [MSI PRO Z690-A](https://www.msi.com/Motherboard/PRO-Z690-A-WIFI-DDR4/Specification)
- RAM: 128GiB DDR4 3200MHz 51.2GB/s
- OS: [Fedora 39](https://docs.fedoraproject.org/en-US/fedora/latest/release-notes/)
  - Kernel version: `6.5.5-300.fc39.x86_64`
- Compiler: Clang/LLVM `main` latest +
  [patch](../../data/patches/LLVM/clang_llvm.patch)
- ROCm Components:
  - `hip-devel`: 5.7.31921.50700-63.el7
  - `rocthrust-devel`: 2.18.0.50700-63.el7
  - `rocprim-devel`: 2.13.1.50700-63.el7
- Standard Library:
  - `libstdc++`: 13.2.1-3.fc39
- TBB: 2020.3-20.fc39

## BabelStream

- [repo](https://github.com/UoB-HPC/BabelStream)
- [patch](../../data/patches/BabelStream/BABELSTREAM.patch)

| Test  | A - CPU (GB/s) | B - GPU (GB/s) | B / A (higher is better) |
|:-----:|:--------------:|:--------------:|:------------------------:|
| Copy  | 33             | 502            | 15                       |
| Mul   | 31             | 502            | 16                       |
| Add   | 33             | 552            | 16                       |
| Triad | 33             | 556            | 16                       |
| Dot   | 40             | 692            | 17                       |

## `cloverleaf_stdpar`

- [repo](https://github.com/UoB-HPC/cloverleaf_stdpar)
- [patch](../../data/patches/cloverleaf_stdpar/CLOVERLEAF_STDPAR.patch)

| Test              | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:-----------------:|:-----------:|:-----------:|:------------------------:|
| clover_bm16_short | 60          | 4           | 15                       |
| clover_bm32_short | 123         | 9           | 13                       |
| clover_bm64_short | 247         | 18          | 13                       |
| clover_qa         | 3           | 0.4         | 7                        |
| clover_sodbig     | 2           | 2           | 1                        |

## CppCon19-stdpar

- [repo](https://github.com/dkolsen-pgi/CppCon19-stdpar)
- [patch](../../data/patches/CppCon19-stdpar/CPPCON19_STDPAR.patch)

| Test    | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:-------:|:-----------:|:-----------:|:------------------------:|
| jacobi1 | 0.42        | 0.32        | 1.3                      |
| jacobi2 | 0.38        | 0.38        | 1                        |

## `cuda_stdpar`

- [repo](https://github.com/quxflux/cuda_stdpar)
- [patch](../../data/patches/cuda_stdpar/CUDA_STDPAR.patch)

| Test            | A - CPU (MItems/s) | B - GPU (MItems/s) | B / A (higher is better) |
|:---------------:|:------------------:|:------------------:|:------------------------:|
| AoS - 9         | 3                  | 0.4                | 0.13                     |
| AoS - 99        | 5                  | 5                  | 1                        |
| AoS - 1000      | 23                 | 44                 | 1.9                      |
| AoS - 9999      | 63                 | 290                | 4                        |
| AoS - 100000    | 456                | 816                | 1.7                      |
| AoS - 999999    | 514                | 23507              | 45                       |
| AoS - 9999999   | 532                | 47112              | 88                       |
| SoA - 9         | 28                 | 0.4                | 0.01                     |
| SoA - 99        | 29                 | 4                  | 0.13                     |
| SoA - 1000      | 28                 | 45                 | 1.6                      |
| SoA - 9999      | 24                 | 289                | 12                       |
| SoA - 100000    | 24                 | 818                | 34                       |
| SoA - 999999    | 24                 | 1019               | 42                       |
| SoA - 9999999   | 25                 | 45300              | 1812                     |

## `cuda-stdpar-samples`

- [repo](https://github.com/heidrifx/cuda-stdpar-samples)
- [patch](../../data/patches/cuda-stdpar-samples/CUDA_STDPAR_SAMPLES.patch)

| Test                      | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:-------------------------:|:------------:|:------------:|:------------------------:|
| daxpy                     | 80           | 1119         | 0.07                     |
| filterMapReduce           | 132          | 609          | 0.21                     |
| filterMapReduce_optimized | 26           | 616          | 0.04                     |
| mandelbrot                | 53803        | 2154         | 24                       |
| saxpy                     | 78           | 1048         | 0.07                     |
| sgemm                     | 5963         | 26124        | 0.22                     |
| vectorAdd                 | 47           | 802          | 0.05                     |
| vectorReduce              | 287          | 865          | 0.33                     |

- notes:
  - run as `./aggregateData_local.sh 3 3 1`
  - results are the average of the 3 runs

## `himeno-stdpar`

- [repo](https://github.com/asix001/himeno-stdpar)
- [patch](../../data/patches/himeno-stdpar/HIMENO.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| SMALL  | 58               | 149              | 2                        |
| MIDDLE | 19               | 195              | 10                       |
| LARGE  | 17               | 190              | 11                       |

## LULESH

- [repo](https://github.com/LLNL/LULESH)
- [patch](../../data/patches/LULESH/LULESH.patch)

| Test               | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:------------------:|:-----------:|:-----------:|:------------------------:|
| Problem size = 30  | 6           | 44          | 0.13                     |
| Problem size = 50  | 19          | 190         | 0.1                      |
| Problem size = 80  | 102         | 1190        | 0.08                     |
| Problem size = 100 | 245         | 2900        | 0.08                     |

## miniBUDE

- [repo](https://github.com/UoB-HPC/miniBUDE)
- [patch](../../data/patches/miniBUDE/MINIBUDE.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| bm1    | 68               | 2008             | 21                       |
| bm2    | 63               | 1814             | 22                       |

## miniWeather

- [repo](https://github.com/jefflarkin/miniWeather)
- [patch](../../data/patches/miniWeather/MINIWEATHER.patch)

| Test            | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------------:|:-----------:|:-----------:|:------------------------:|
| THERMAL         | 14          | 4           | 3                        |
| COLLISION       | 14          | 4           | 3                        |
| MOUNTAIN        | 14          | 4           | 3                        |
| DENSITY_CURRENT | 14          | 4           | 3                        |
| INJECTION       | 14          | 4           | 3                        |

- notes:
  - run parameterised with `NX == 400`, `NZ == 200`, `OUT_FREQ == 10` and
    `SIM_TIME == 1000` as specified in the documentation

## `nbody-stdpar`

- [repo](https://github.com/jbeisman/nbody-stdpar)
- [patch](../../data/patches/nbody-stdpar/NBODY_STDPAR.patch)

| Test         | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:------------:|:------------:|:------------:|:------------------------:|
| 4096 bodies  | 2380         | 2993         | 0.8                      |
| 8192 bodies  | 8190         | 5829         | 1.4                      |
| 16384 bodies | 31152        | 15417        | 2                        |
| 32768 bodies | 125688       | 40990        | 3                        |
| 65536 bodies | 505660       | 90263        | 5.6                      |

- notes:
  - run parameterised with `Simulation duration == 100` and
    `Integration timestep == 0.1`

## `nvstdpar`

- [repo](https://github.com/mhaseeb123/nvstdpar)
- [patch](../../data/patches/nvstdpar/NVSTDPAR.patch)

| Test                  | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------------------:|:-----------:|:-----------:|:------------------------:|
| 1D_STENCIL            | 103         | 8           | 13                       |
| COMM-STUDY-POINTERS   | 2           | 11          | 0.2                      |
| COMM-STUDY-VECTORS    | 2           | 17          | 0.1                      |
| FFT - 536870912       | 17          | 50          | 0.3                      |
| HEAT-EQUATION         | 590         | 50          | 12                       |

- notes:
  - tests parameterised as follows:
    - `1D_STENCIL`: `--size 500000000 --nt 400`
    - `COMM-STUDY-*`: default
    - `FFT`: `-N 536870912 --time`
    - `HEAT-EQUATION`: `-n=23000 -s=1000 --time`
  - comparisons are based on the `stdpar` variants on the code, with the CPU
    and the GPU executing the same source

## p2r-tests

- [repo](https://github.com/cerati/p2r-tests)
- [patch](../../data/patches/p2r-tests/P2R_TESTS.patch)

| Test       | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:----------:|:-----------:|:-----------:|:------------------------:|
| 8192 trks  | 2           | 3           | 0.6                      |
| 16384 trks | 5           | 6           | 0.8                      |
| 32768 trks | 12          | 12          | 1                        |
| 65536 trks | 21          | 24          | 0.8                      |

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
| heat3d | 9                | 112              | 12                       |

| Test   | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:------:|:-----------:|:-----------:|:------------------------:|
| vlp4d  | 235         | 82          | 3                        |

- notes:
  - tests run with the configurations suggested in the
    [readme](https://github.com/yasahi-hpc/P3-miniapps#run)

## Palabos

- [repo](https://gitlab.com/unigehpfs/palabos)
- [patch](../../data/patches/palabos/PALABOS.patch)

| Test             | A - CPU (MLUPS) | B - GPU (MLUPS) | B / A (higher is better) |
|:----------------:|:---------------:|:---------------:|:------------------------:|
| rayleighTaylor3D | 16              | 100             | 6                        |
| sandstone        | 67              | 575             | 8                        |
| tgv3d            | 141             | 1657            | 11                       |

## `paralg`

- [repo](https://gitlab.com/unigehpfs/paralg)
- [patch](../../data/patches/paralg/PARALG.patch)

| Test           | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:--------------:|:----------------:|:----------------:|:------------------------:|
| paralg_column  | 16               | 2.5              | 0.15                     |
| paralg_element | 16               | 124              | 8                        |

## parSTL

- [repo](https://github.com/weilewei/parSTL)
- [patch](../../data/patches/parSTL/PARSTL.patch)

| Test             | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:----------------:|:------------:|:------------:|:------------------------:|
| sort      - 10^5 | 0.08         | 2            | 0.04                     |
| sort      - 10^6 | 0.74         | 18           | 0.04                     |
| sort      - 10^7 | 14           | 12           | 1.1                      |
| sort      - 10^8 | 162          | 103          | 1.57                     |
| transform - 10^5 | 0.12         | 24           | 0.005                    |
| transform - 10^6 | 0.64         | 25           | 0.02                     |
| transform - 10^7 | 5            | 33           | 0.15                     |
| transform - 10^8 | 50           | 101          | 0.49                     |

## pp-EnKF

- [repo](https://github.com/yasahi-hpc/pp-EnKF)
- [patch](../../data/patches/pp-EnKF/PP_ENKF.patch)

| Test                 | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:--------------------:|:-----------:|:-----------:|:------------------------:|
| heat3d               | 144         | 12          | 12                       |
| lbm2d-letkf nature   | 147         | 51          | 2.7                      |
| lbm2d-letkf no_da    | 16          | 0.4         | 40                       |
| lbm2d-letkf nudging  | 116         | 21          | 4                        |
| vlp4d SLD10_large    | 275         | 18          | 15                       |

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
| twopop_aos       | 48              | 262             | 5                        |
| twopop_soa       | 78              | 802             | 10                       |
| swap_aos         | 53              | 114             | 2                        |
| swap_soa         | 62              | 479             | 7                        |
| aa_aos           | 86              | 257             | 3                        |
| aa_soa           | 124             | 651             | 5                        |

## TeaLeaf

- [repo](https://github.com/UoB-HPC/TeaLeaf)
- [patch](../../data/patches/TeaLeaf/TEALEAF.patch)

| Test            | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------------:|:-----------:|:-----------:|:------------------------:|
| tea_bm_4        | 6           | 3.6         | 1.6                      |
| tea_bm_5        | 1890        | 191         | 10                       |
| tea_bm_5e_1     | 6           | 3.6         | 1.6                      |
| tea_bm_5e_2     | 205         | 22          | 9                        |
| tea_bm_5e_4     | 1890        | 191         | 10                       |

## `tsp`

- [repo](https://github.com/pkestene/tsp)
- [patch](../../data/patches/tsp/TSP.patch)

| Test      | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------:|:-----------:|:-----------:|:------------------------:|
| 10 cities | 0.007       | 0.12        | 0.05                     |
| 11 cities | 0.027       | 0.13        | 0.2                      |
| 12 cities | 0.25        | 0.28        | 0.9                      |
| 13 cities | 4           | 3           | 1.3                      |
| 14 cities | 62          | 44          | 1.4                      |

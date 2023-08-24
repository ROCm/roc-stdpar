# Performance Study: Intel Core I9-13900KF && AMD Radeon VII

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

- CPU: [Intel Core I9-13900KF](https://www.techpowerup.com/cpu-specs/core-i9-13900kf.c2916)
- GPU: [AMD Radeon VII](https://www.techpowerup.com/gpu-specs/radeon-vii.c3358)
- Motherboard: [MSI PRO Z690-A](https://www.msi.com/Motherboard/PRO-Z690-A-WIFI-DDR4/Specification)
- RAM: 128GiB DDR4 3200MHz 51.2GB/s
- OS: [Fedora 38](https://docs.fedoraproject.org/en-US/fedora/latest/release-notes/)
  - Kernel version: `6.4.6-200.fc38.x86_64`
- Compiler: Clang/LLVM `main` latest +
  [patch](../../data/patches/LLVM/clang_llvm.patch)
- ROCm Components:
  - `rocm-hip`: 5.5.1-8.fc38
  - `rocthrust-devel`: 2.10.9.50501-74.el7
  - `rocprim-devel`: 2.10.9.50501-74.el7
- Standard Library:
  - `libstdc++`: 13.1.1-4.fc38
- TBB: 2020.3-16.fc38

## BabelStream

- [repo](https://github.com/UoB-HPC/BabelStream)
- [patch](../../data/patches/BabelStream/BABELSTREAM.patch)

| Test  | A - CPU (GB/s) | B - GPU (GB/s) | B / A (higher is better) |
|:-----:|:--------------:|:--------------:|:------------------------:|
| Copy  | 33             | 690            | 20                       |
| Mul   | 32             | 687            | 21                       |
| Add   | 33             | 686            | 20                       |
| Triad | 34             | 688            | 20                       |
| Dot   | 41             | 744            | 18                       |

## `cloverleaf_stdpar`

- [repo](https://github.com/UoB-HPC/cloverleaf_stdpar)
- [patch](../../data/patches/cloverleaf_stdpar/CLOVERLEAF_STDPAR.patch)

| Test              | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:-----------------:|:-----------:|:-----------:|:------------------------:|
| clover_bm16_short | 58          | 4           | 14                       |
| clover_bm32_short | 117         | 8           | 14                       |
| clover_bm64_short | 233         | 16          | 14                       |
| clover_qa         | 3           | 0.3         | 10                       |
| clover_sodbig     | 3           | 2           | 1.5                      |

## CppCon19-stdpar

- [repo](https://github.com/dkolsen-pgi/CppCon19-stdpar)
- [patch](../../data/patches/CppCon19-stdpar/CPPCON19_STDPAR.patch)

| Test    | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:-------:|:-----------:|:-----------:|:------------------------:|
| jacobi1 | 0.4         | 2           | 0.2                      |
| jacobi2 | 0.3         | 4           | 0.07                     |

## `cuda_stdpar`

- [repo](https://github.com/quxflux/cuda_stdpar)
- [patch](../../data/patches/cuda_stdpar/CUDA_STDPAR.patch)

| Test            | A - CPU (MItems/s) | B - GPU (MItems/s) | B / A (higher is better) |
|:---------------:|:------------------:|:------------------:|:------------------------:|
| AoS - 9         | 21                 | 0.4                | 0.01                     |
| AoS - 99        | 32                 | 4                  | 0.12                     |
| AoS - 1000      | 67                 | 42                 | 0.62                     |
| AoS - 9999      | 344                | 291                | 0.84                     |
| AoS - 100000    | 186                | 827                | 6                        |
| AoS - 999999    | 668                | 22859              | 34                       |
| AoS - 9999999   | 697                | 44306              | 63                       |
| SoA - 9         | 33                 | 0.1                | 0.003                    |
| SoA - 99        | 43                 | 4                  | 0.09                     |
| SoA - 1000      | 36                 | 43                 | 1.1                      |
| SoA - 9999      | 31                 | 279                | 9                        |
| SoA - 100000    | 31                 | 831                | 26                       |
| SoA - 999999    | 31                 | 1037               | 33                       |
| SoA - 9999999   | 31                 | 42992              | 1386                     |

## `cuda-stdpar-samples`

- [repo](https://github.com/heidrifx/cuda-stdpar-samples)
- [patch](../../data/patches/cuda-stdpar-samples/CUDA_STDPAR_SAMPLES.patch)

| Test                      | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:-------------------------:|:------------:|:------------:|:------------------------:|
| daxpy                     | 79           | 2894         | 0.02                     |
| filterMapReduce           | 131          | 535          | 0.24                     |
| filterMapReduce_optimized | 25           | 539          | 0.1                      |
| mandelbrot                | 53028        | 1977         | 26                       |
| saxpy                     | 78           | 956          | 0.08                     |
| sgemm                     | 5986         | 23003        | 0.26                     |
| vectorAdd                 | 47           | 745          | 0.06                     |
| vectorReduce              | 290          | 818          | 0.35                     |

- notes:
  - run as `./aggregateData_local.sh 3 3 1`
  - results are the average of the 3 runs

## `himeno-stdpar`

- [repo](https://github.com/asix001/himeno-stdpar)
- [patch](../../data/patches/himeno-stdpar/HIMENO.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| SMALL  | 58               | 172              | 3                        |
| MIDDLE | 19               | 214              | 11                       |
| LARGE  | 17               | 194              | 11                       |

## miniBUDE

- [repo](https://github.com/UoB-HPC/miniBUDE)
- [patch](../../data/patches/miniBUDE/MINIBUDE.patch)

| Test   | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:------:|:----------------:|:----------------:|:------------------------:|
| bm1    | 67               | 2108             | 31                       |
| bm2    | 62               | 1906             | 30                       |

## miniWeather

- [repo](https://github.com/jefflarkin/miniWeather)
- [patch](../../data/patches/miniWeather/MINIWEATHER.patch)

| Test            | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------------:|:-----------:|:-----------:|:------------------------:|
| THERMAL         | 13          | 4           | 3                        |
| COLLISION       | 9           | 3           | 3                        |
| MOUNTAIN        | 20          | 6           | 3                        |
| DENSITY_CURRENT | 8           | 2           | 4                        |
| INJECTION       | 16          | 5           | 3                        |

- notes:
  - run parameterised with `NX == 400`, `NZ == 200`, `OUT_FREQ == 10` and
    `SIM_TIME` as specified in the documentation

## `nbody-stdpar`

- [repo](https://github.com/jbeisman/nbody-stdpar)
- [patch](../../data/patches/nbody-stdpar/NBODY_STDPAR.patch)

| Test         | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:------------:|:------------:|:------------:|:------------------------:|
| 4096 bodies  | 2192         | 2943         | 0.7                      |
| 8192 bodies  | 8054         | 5831         | 1.3                      |
| 16384 bodies | 32011        | 11147        | 2.8                      |
| 32768 bodies | 127887       | 36974        | 3.4                      |
| 65536 bodies | 510415       | 87578        | 5.8                      |

- notes:
  - run parameterised with `Simulation duration == 100` and
    `Integration timestep == 0.1`

## p2r-tests

- [repo](https://github.com/cerati/p2r-tests)
- [patch](../../data/patches/p2r-tests/P2R_TESTS.patch)

| Test       | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:----------:|:-----------:|:-----------:|:------------------------:|
| 8192 trks  | 3           | 3           | 1                        |
| 16384 trks | 6           | 6           | 1                        |
| 32768 trks | 14          | 12          | 1.16                     |
| 65536 trks | 23          | 23          | 1                        |

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
| heat3d | 9                | 128              | 14                       |

| Test   | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:------:|:-----------:|:-----------:|:------------------------:|
| vlp4d  | 251         | 16          | 15                       |

- notes:
  - CPU testing was done using GCC13.1, due to the OpenMP requirements
  - tests run with the configurations suggested in the
    [readme](https://github.com/yasahi-hpc/P3-miniapps#run)
  - `vlp4d` temporarily requires using Interpose Mode via the
    `--stdpar-interpose-alloc` flag

## Palabos

- [repo](https://gitlab.com/unigehpfs/palabos)
- [patch](../../data/patches/palabos/PALABOS.patch)

| Test             | A - CPU (MLUPS) | B - GPU (MLUPS) | B / A (higher is better) |
|:----------------:|:---------------:|:---------------:|:------------------------:|
| rayleighTaylor3D | 16              | 126             | 7                        |
| sandstone        | 68              | 1104            | 16                       |
| tgv3d            | 142             | 3073            | 21                       |

## `paralg`

- [repo](https://gitlab.com/unigehpfs/paralg)
- [patch](../../data/patches/paralg/PARALG.patch)

| Test           | A - CPU (GFLOPS) | B - GPU (GFLOPS) | B / A (higher is better) |
|:--------------:|:----------------:|:----------------:|:------------------------:|
| paralg_column  | 16               | 0.81             | 0.01                     |
| paralg_element | 16               | 147              | 9                        |

## parSTL

- [repo](https://github.com/weilewei/parSTL)
- [patch](../../data/patches/parSTL/PARSTL.patch)

| Test             | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:----------------:|:------------:|:------------:|:------------------------:|
| sort      - 10^5 | 0.07         | 2            | 0.03                     |
| sort      - 10^6 | 0.7          | 18           | 0.03                     |
| sort      - 10^7 | 13           | 9            | 1.4                      |
| sort      - 10^8 | 163          | 94           | 1.7                      |
| transform - 10^5 | 0.19         | 21           | 0.009                    |
| transform - 10^6 | 0.58         | 22           | 0.02                     |
| transform - 10^7 | 5            | 28           | 0.17                     |
| transform - 10^8 | 53           | 89           | 0.5                      |

## STLBM - Lattice Boltzmann simulation framework based on C++ Parallel Algorithms

- [repo](https://gitlab.com/unigehpfs/stlbm)
- [patch](../../data/patches/stlbm/STLBM.patch)

| Test             | A - CPU (MLUPS) | B - GPU (MLUPS) | B / A (higher is better) |
|:----------------:|:---------------:|:---------------:|:------------------------:|
| twopop_aos       | 49              | 432             | 8                        |
| twopop_soa       | 78              | 777             | 9                        |
| swap_aos         | 53              | 189             | 3                        |
| swap_soa         | 62              | 544             | 8                        |
| aa_aos           | 86              | 316             | 3                        |
| aa_soa           | 124             | 695             | 5                        |

## `traccc`

- [repo](https://github.com/ugGit/traccc)
- [patch](../../data/patches/traccc/TRACCC.patch)

| Test                               | A - CPU (ms) | B - GPU (ms) | A / B (higher is better) |
|:----------------------------------:|:------------:|:------------:|:------------------------:|
| cca_stdpar_simplified_sv           | 22           | 1.5          | 14                       |
| cca_stdpar_fast_sv_1               | 24           | 1.5          | 16                       |
| cca_stdpar_fast_sv_2               | 24           | 1.5          | 16                       |
| cca_stdpar_fast_sv_2_partition_128 | 24           | 1.4          | 17                       |
| cca_stdpar_fast_sv_2_partition_256 | 24           | 1.4          | 17                       |
| cca_stdpar_fast_sv_2_partition_512 | 24           | 1.4          | 17                       |

- notes:
  - selected `tml_full/ttbar_mu300/` test case results

## `tsp`

- [repo](https://github.com/pkestene/tsp)
- [patch](../../data/patches/tsp/TSP.patch)

| Test      | A - CPU (s) | B - GPU (s) | A / B (higher is better) |
|:---------:|:-----------:|:-----------:|:------------------------:|
| 10 cities | 0.006       | 0.1         | 0.06                     |
| 11 cities | 0.027       | 0.12        | 0.2                      |
| 12 cities | 0.25        | 0.35        | 0.7                      |
| 13 cities | 4           | 3           | 1.3                      |
| 14 cities | 62          | 48          | 1.3                      |

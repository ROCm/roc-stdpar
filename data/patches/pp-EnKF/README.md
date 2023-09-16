# pp-EnKF

- url: <https://github.com/yasahi-hpc/pp-EnKF>
- branch: `main` at `b4f25bd7a949dc3b5362a1bbe6a2c73a5f8a35b2`
- notes:
  - the value `CLANG` is added to the set of possible values for the `BACKEND`
    CMake variable, and it's used to select Clang/LLVM Standard Algorithm
    offloading when the `STDPAR` value passed to CMake via the
    `PROGRAMMING_MODEL` variable
  - the `__HIPSTDPAR__` macro is used to control conditional compilation
    equivalently to `_NVHPC_STDPAR_GPU`
  - only the hipFFT specialised library is used
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload

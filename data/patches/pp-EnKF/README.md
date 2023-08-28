# pp-EnKF

- url: <https://github.com/yasahi-hpc/pp-EnKF>
- branch: `main` at `e446499c2923b80ab2bb96b27c8ef062e3101091`
- notes:
  - the value `CLANG` is added to the set of possible values for the `BACKEND`
    CMake variable, and it's used to select Clang/LLVM Standard Algorithm
    offloading when the `STDPAR` value passed to CMake via the
    `PROGRAMMING_MODEL` variable
  - the `__HIPSTDPAR__` macro is used to control conditional compilation
    equivalently to `_NVHPC_STDPAR_GPU`
  - no HIP specialised libraries are used
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload

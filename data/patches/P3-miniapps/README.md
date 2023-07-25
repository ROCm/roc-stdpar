# P3-miniapps

- url: <https://github.com/yasahi-hpc/P3-miniapps>
- branch: `main`
- notes:
  - the value `CLANG` is added to the set of possible values for the `BACKEND`
    CMake variable, and it's used to select Clang/LLVM Standard Algorithm
    offloading when the `STDPAR` value passed to CMake via the
    `PROGRAMMING_MODEL` variable
  - the `__STDPAR__` macro is used to control conditional compilation
    equivalently to `_NVHPC_STDPAR_GPU`
  - HIP equivalents are used for FFTs and transpositions when usinc Clang/LLVM
    offload
  - functionality relying on views being `CopyConstructible` is temporarily
    disabled
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

# P3-miniapps

- url: <https://github.com/yasahi-hpc/P3-miniapps>
- branch: `main` at `c1b946b91b608929ac7306e95e01222f726cdb5d`
- notes:
  - the value `CLANG` is added to the set of possible values for the `BACKEND`
    CMake variable, and it's used to select Clang/LLVM Standard Algorithm
    offloading when the `STDPAR` value passed to CMake via the
    `PROGRAMMING_MODEL` variable
  - the `__HIPSTDPAR__` macro is used to control conditional compilation
    equivalently to `_NVHPC_STDPAR_GPU`
  - HIP equivalents are used for FFTs and transpositions when usinc Clang/LLVM
    offload
  - functionality relying on views being `CopyConstructible` is temporarily
    disabled
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for the `vlp4d` miniapp interposition mode is temporarily the deault

# `cuda-stdpar-samples`

- url: <https://github.com/heidrifx/cuda-stdpar-samples>
- branch: `main` at `9972980f81c338f52d23cb81733a279b2c15109b`
- notes:
  - a build script named `compile_local_clang_stdpar.sh` is added
  - the following environment variables are considered by the script:
    - `STDPAR_CXX`: specifies the compiler to use, which must be Clang/LLVM with
       support for Standard Algorithm offloading
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload

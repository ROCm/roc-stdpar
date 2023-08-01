# `p2r-tests`

- url: <https://github.com/cerati/p2r-tests>
- branch: `main` at `88e88ab3814dfc535eba8c31970f985eff8681b8`
- notes:
  - the `build.py` build script is extended with `clang` and `clang_gpu`
    compilers, both of which apply to the `pstl` technology
  - the following variables are considered by the build script iff the
    `clang_gpu` compiler is chosen:
    - `STDPAR_CXX`: specifies the compiler to use, which must be Clang/LLVM with
      support for Standard Algorithm offloading
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - the CMake based build infrastructure is extended by adding two targets,
    `clang_stdpar_cpu` and `clang_stdpar_gpu`
  - the following variables are considered by CMake, iff compiling with
     Clang / LLVM:
    - `CLANG_STDPAR`: specifies whether or not offload should be enabled by the
       toolchain
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

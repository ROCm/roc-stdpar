# `traccc`

- url: <https://github.com/ugGit/traccc>
- branch: `main` at `6c6572f16fcf7e8136db85059becdcd8a0fcb604`
- notes:
  - only the algorithm embodied by `fast_sv_kernel` is currently offloaded
  - the 'Detray' component has a dependency on CUDA Thrust that it opaquely
    injects into the set of build dependencies, which creates a conflict; for
    the moment it is necessary to manually remove the dependence on the
    imported library `detray::thrust`
  - the `clang` value is added to the set of possible values of the
    `TRACCC_STDPAR_BACKEND`, do denote offload via Clang/LLVM
  - the following variables are added to CMake:
    - `TRACCC_STDPAR_PATH`: specifies the path to the forwarding header
    - `TRACCC_STDPAR_TARGET`: specifies the target for algorithm offload

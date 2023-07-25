# `stdpar-mandelbrot`

- url: <https://github.com/UoB-HPC/stdpar-mandelbrot>
- branch: `main`
- notes:
  - the following variables are added to CMake:
    - `CLANG_STDPAR_PATH`: specifies the path to the forwarding header
    - `CLANG_OFFLOAD_GPU`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

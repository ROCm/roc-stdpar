# `cuda_stdpar`

- url: <https://github.com/quxflux/cuda_stdpar>
- branch: `master` at `bbfc7517242a27cf2e5c012d7d1530d40107a6a7`
- notes:
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

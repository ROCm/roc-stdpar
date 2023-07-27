# `cloverleaf_stdpar`

- url: <https://github.com/UoB-HPC/cloverleaf_stdpar>
- branch: `main` at `3024ee70d4097f324c1eb1e454adee24209a6a42`
- notes:
  - the following variables are added to CMake:
    - `CLANG_STDPAR_PATH`: specifies the path to the forwarding header
    - `CLANG_OFFLOAD`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

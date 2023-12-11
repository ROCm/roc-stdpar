# TeaLeaf

- url: <https://github.com/UoB-HPC/TeaLeaf>
- branch: `master` at `5ee7d753bbb8e8d60b945c0359e00c07aafbab81`
- notes:
  - the following variables are added to CMake:
    - `CLANG_STDPAR_PATH`: specifies the path to the forwarding header
    - `CLANG_OFFLOAD`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

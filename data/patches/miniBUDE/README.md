# miniBUDE

- url: <https://github.com/UoB-HPC/miniBUDE>
- branch: `v2` at `bea30762acaefee54ebaf3c68713b66414345e12`
- notes:
  - the following variables are added to CMake:
    - `CLANG_STDPAR_PATH`: specifies the path to the forwarding header
    - `CLANG_OFFLOAD`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

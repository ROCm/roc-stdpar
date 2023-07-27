# BabelStream

- url: <https://github.com/UoB-HPC/BabelStream>
- branch: `main` at `7d570547edf0f0222d022d8cd1d67f768e666738`
- notes:
  - the following variables are added to CMake:
    - `CLANG_STDPAR_PATH`: specifies the path to the forwarding header
    - `CLANG_OFFLOAD`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

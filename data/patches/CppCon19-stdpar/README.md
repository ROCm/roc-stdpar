# CppCon19-stdpar

- url: <https://github.com/dkolsen-pgi/CppCon19-stdpar>
- branch: `main` at `71e4305907b2710def4ef88a036980a9e08529e2`
- notes:
  - execution policies are switched from `par` to `par_unseq` to enable offload
  - a minimalist CMake based build infrastructure is added
  - the following variables are considered by CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

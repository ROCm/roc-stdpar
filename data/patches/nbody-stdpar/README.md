# `nbody-stdpar`

- url: <https://github.com/jbeisman/nbody-stdpar>
- branch: `master` at `3253e13d45222582ef3134c8177030b9837ba35d`
- notes:
  - a minimalist CMake based build infrastructure is added
  - the following variables are considered by CMake:
    - `USE_OFFLOAD`: specifies whether or not offload should be enabled by the
       toolchain
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

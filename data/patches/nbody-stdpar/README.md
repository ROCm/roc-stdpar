# `nbody-stdpar`

- url: <https://github.com/jbeisman/nbody-stdpar>
- branch: `master` at `e675e4cd7acea872dbe8cd3f7a92ad362f4f76f7`
- notes:
  - the following variables are added to CMake:
    - `ENABLE_AMD_GPU`: specifies whether or not offload for AMD GPUs should be
      enabled
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

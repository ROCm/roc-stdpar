# `nvstdpar`

- url: <https://github.com/mhaseeb123/nvstdpar>
- branch: `main` at `d7ac353a94fcca07f4670c1f2caffcbd2ce7b090`
- notes:
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

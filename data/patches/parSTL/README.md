# parSTL

- url: <https://github.com/weilewei/parSTL>
- branch: `master` at `c385e2ee8359a5e046fa50e3e08a50e97a482285`
- notes:
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

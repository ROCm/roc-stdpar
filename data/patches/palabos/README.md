# Palabos

- url: <https://gitlab.com/unigehpfs/palabos>
- branch: `master` at `13bc5ba4d03306367ef65983aceaf0185a535c91`
- notes:
  - the three Standard Parallelism examples under `examples/80days` must be
    built individually, and are self-contained
  - `printf` support is temporarily disabled
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

# STLBM - Lattice Boltzmann simulation framework based on C++ Parallel Algorithms

- url: <https://gitlab.com/unigehpfs/stlbm>
- branch: `master` at `326d7175ced0b6c84b9f1d574eebc66385de589e`
- notes:
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

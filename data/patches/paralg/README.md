# `paralg`

- url: <https://gitlab.com/unigehpfs/paralg>
- branch: `master`
- notes:
  - execution policies are switched from `par` to `par_unseq` to enable offload
  - a `nompi_clang` target is added to the Makefile
  - the following variables are incorporated into the Makefile:
    - `STDPAR_CXX`: specifies the compiler to use, which must be Clang/LLVM
      with support for Standard Algorithm offloading
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

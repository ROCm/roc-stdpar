# LULESH

- url: <https://github.com/LLNL/LULESH>
- branch: `2.0.2-dev`
- notes:
  - execution policies are switched from `par` to `par_unseq` to enable offload
  - a Makefile named `Makefile-stdpar` is added
    - the following environment variables are considered by the Makefile:
      - `STDPAR_CXX`: specifies the compiler to use, which must be Clang/LLVM
         with support for Standard Algorithm offloading
      - `STDPAR_PATH`: specifies the path to the forwarding header
      - `STDPAR_TARGET`: specifies the target for algorithm offload

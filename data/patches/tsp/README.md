# `tsp`

- url: <https://github.com/pkestene/tsp>
- branch: `master` at `515872774fdeb5caf2ee13cbf1d64e3720e8995b`
- notes:
  - execution policies are switched from `par` to `par_unseq` to enable offload
  - a `tsp_clang_stdpar_cpu` target is added to the Makefile
  - a `tsp_clang_stdpar_gpu` target is added to the Makefile
  - the following variables are incorporated into the Makefile:
    - `STDPAR_CXX`: specifies the compiler to use, which must be Clang/LLVM
      with support for Standard Algorithm offloading
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload

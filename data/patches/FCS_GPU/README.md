# FCS-GPU

- url: <https://github.com/cgleggett/FCS-GPU>
- branch: `main` at `f787d52fb292db127d265c00b48f938a5b9dd9cf`
- notes:
  - **WARNING** this is work in progress and has not been fully validated due to
    missing access to the expected testing data set
  - the unguarded dependencies on CUDA that exist in multiple places are made
    modal on the `__STDPAR__` macro and pick HIP when the latter is defined
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_OFFLOAD_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled
  - `-fcf-protection=none` is temporarily set globally, due to a temporary
    limitation and the complexity of the build infrastructure making it
    impossible to disable locally

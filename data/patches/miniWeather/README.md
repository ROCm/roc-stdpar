# miniWeather

- url: <https://github.com/jefflarkin/miniWeather>
- branch: `cppstdpar` at `cfa6243bc22acf4fe9863bb172adc398dd4377a2`
- notes:
  - execution policies are switched from `par` to `par_unseq` to enable offload
  - the following variables are added to CMake:
    - `STDPAR_PATH`: specifies the path to the forwarding header
    - `STDPAR_TARGET`: specifies the target for algorithm offload
  - for targets that are not part of the GFX9 family interposition mode is
    implicitly enabled

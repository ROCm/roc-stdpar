# Description

This folder contains a series of patches that should cleanly apply to:

- LLVM's main branch, to add compiler support for standard algorithm offload;
- The main branches of a series of client applications which have adopted
  standard algorithm offload as offered by toolchains other than Clang/LLVM in
  the past.

The LLVM patch is required, and will be re-synced weekly until the upstream
review concludes and functionality is merged. Repository information and
eventual notes are provided for each of the client applications.
# docker image

- builds an Ubuntu 22.04 docker image with AMD ROCm, patched LLVM, roc-stdpar, and example workloads
- to adjust offload target, change `ENV OFFLOAD_ARCH=gfx908` to, e.g., `ENV OFFLOAD_ARCH=gfx90a`
- to build: `DOCKER_BUILDKIT=1 BUILDKIT_STEP_LOG_MAX_SIZE=20m BUILDKIT_PROGRESS=plain docker build .`

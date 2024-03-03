FROM ubuntu:22.04

LABEL org.opencontainers.image.authors="johannes.dieterich@amd.com"

ENV TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt update
RUN apt upgrade -y
# generic things
RUN apt install -y sudo wget gnupg2 git gcc bzip2 cmake build-essential numactl vim ninja-build mold opencl-clhpp-headers opencl-c-headers libtbb-dev clinfo libpthread-stubs0-dev libboost-dev flex build-essential bison libboost-all-dev numactl sqlite3

# add ROCm repo
RUN wget -q -O - https://repo.radeon.com/rocm/rocm.gpg.key | sudo apt-key add -
RUN echo 'deb [arch=amd64] https://repo.radeon.com/rocm/apt/debian/ ubuntu main' | sudo tee /etc/apt/sources.list.d/rocm.list
RUN apt update
RUN apt install -y rocthrust5.6.0

ADD target.lst /opt/rocm/bin/

# install dependencies
RUN apt install -y cmake make gcc rocm-cmake5.6.0 rocm-hip-runtime-dev5.6.0 libelf-dev

# clone roc-stdpar
WORKDIR /root
RUN git clone https://github.com/ROCmSoftwarePlatform/roc-stdpar

# clone upstream LLVM, patch, and build
WORKDIR /root
RUN git clone https://github.com/llvm/llvm-project.git
WORKDIR llvm-project
RUN git checkout 0058c92609a43079420668877c09948811e94d44
RUN git apply /root/roc-stdpar/data/patches/LLVM/CLANG_LLVM.patch
RUN cmake -S llvm -B build -G Ninja -DCMAKE_INSTALL_PREFIX=/opt -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang;lld;openmp" -DLLVM_USE_LINKER="mold"
RUN cmake --build build -j$(nproc)
RUN cmake --build build --target install -j$(nproc) 

ENV OFFLOAD_ARCH=gfx908
ARG STDPAR_PATH=/root/roc-stdpar/include
ARG STDPAR_TARGET=${OFFLOAD_ARCH}
ARG STDPAR_CXX=/opt/bin/clang++

# BabelStream
WORKDIR /root
RUN git clone https://github.com/UoB-HPC/BabelStream.git
WORKDIR BabelStream
RUN git checkout 7d570547edf0f0222d022d8cd1d67f768e666738 && git apply /root/roc-stdpar/data/patches/BabelStream/BABELSTREAM.patch
RUN cmake -B build -DCLANG_OFFLOAD=${OFFLOAD_ARCH} -DCLANG_STDPAR_PATH=/root/roc-stdpar/include -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCMAKE_PREFIX_PATH=/opt/rocm-5.6.0/lib/cmake/ -DMODEL="std-data;std-indices" -DCMAKE_BUILD_TYPE=Release .
RUN cmake --build build -j$(nproc)

# CppCon19-stdpar
WORKDIR /root
RUN git clone https://github.com/dkolsen-pgi/CppCon19-stdpar.git
WORKDIR CppCon19-stdpar
RUN git checkout 71e4305907b2710def4ef88a036980a9e08529e2 && git apply /root/roc-stdpar/data/patches/CppCon19-stdpar/CPPCON19_STDPAR.patch
#RUN cmake -B build -DCLANG_OFFLOAD=${OFFLOAD_ARCH} -DCLANG_STDPAR_PATH=/root/roc-stdpar/include -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCMAKE_BUILD_TYPE=Release ..
#RUN cmake --build build -j$(nproc)
# TODO

# FCS_GPU
WORKDIR /root
RUN git clone https://github.com/cgleggett/FCS-GPU.git
WORKDIR FCS-GPU
RUN git checkout f787d52fb292db127d265c00b48f938a5b9dd9cf
#RUN git apply /root/roc-stdpar/data/patches/FCS_GPU/FCS_GPU.patch
#TODO

# LULESH
WORKDIR /root
RUN git clone https://github.com/LLNL/LULESH.git
WORKDIR LULESH
RUN git checkout 46c0ffcc503c300ed3ef364103499958a5c4ee16 && git apply /root/roc-stdpar/data/patches/LULESH/LULESH.patch
WORKDIR stdpar/build
RUN make -f Makefile-stdpar

# P3 miniapps
WORKDIR /root
RUN git clone https://github.com/yasahi-hpc/P3-miniapps.git
WORKDIR P3-miniapps
RUN git checkout c1b946b91b608929ac7306e95e01222f726cdb5d
#RUN git apply /root/roc-stdpar/data/patches/P3-miniapps/P3_MINIAPPS.patch
# TODO

# cloverleaf stdpar
WORKDIR /root
RUN git clone https://github.com/UoB-HPC/cloverleaf_stdpar.git
WORKDIR cloverleaf_stdpar
RUN git checkout 3024ee70d4097f324c1eb1e454adee24209a6a42 && git apply /root/roc-stdpar/data/patches/cloverleaf_stdpar/CLOVERLEAF_STDPAR.patch
#RUN cmake -B build -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCLANG_OFFLOAD=${OFFLOAD_ARCH} -DCLANG_STDPAR_PATH=/root/roc-stdpar/include -DCMAKE_BUILD_TYPE=Release .
#RUN cmake --build build -j$(nproc)
# TODO

# cuda stdpar samples
WORKDIR /root
RUN git clone https://github.com/heidrifx/cuda-stdpar-samples.git
WORKDIR cuda-stdpar-samples
RUN git checkout 9972980f81c338f52d23cb81733a279b2c15109b && git apply /root/roc-stdpar/data/patches/cuda-stdpar-samples/CUDA_STDPAR_SAMPLES.patch
RUN bash compile_local_clang_stdpar.sh

# cuda stdpar
WORKDIR /root
RUN git clone https://github.com/quxflux/cuda_stdpar.git
WORKDIR cuda_stdpar
RUN git checkout bbfc7517242a27cf2e5c012d7d1530d40107a6a7 && git apply /root/roc-stdpar/data/patches/cuda_stdpar/CUDA_STDPAR.patch
RUN cmake -B build -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCLANG_OFFLOAD=${OFFLOAD_ARCH} -DCLANG_STDPAR_PATH=/root/roc-stdpar/include -DCMAKE_BUILD_TYPE=Release .
#RUN cmake --build build -j$(nproc)
# TODO

# himeno-stdpar
WORKDIR /root
RUN git clone https://github.com/asix001/himeno-stdpar.git
WORKDIR himeno-stdpar
RUN git checkout fe8f8b61d861a4b839a14edf7295b119d9e424e3 && git apply /root/roc-stdpar/data/patches/himeno-stdpar/HIMENO.patch
WORKDIR stdpar
RUN ${STDPAR_CXX} -std=c++17 -stdpar --stdpar-path=${STDPAR_PATH} --offload-arch=${OFFLOAD_ARCH} -DLARGE himenoBMTxps.cpp -o himenoBMTxps_large
RUN ${STDPAR_CXX} -std=c++17 -stdpar --stdpar-path=${STDPAR_PATH} --offload-arch=${OFFLOAD_ARCH} -DMIDDLE himenoBMTxps.cpp -o himenoBMTxps_middle
RUN ${STDPAR_CXX} -std=c++17 -stdpar --stdpar-path=${STDPAR_PATH} --offload-arch=${OFFLOAD_ARCH} -DSMALL himenoBMTxps.cpp -o himenoBMTxps_small

# miniBUDE
WORKDIR /root
RUN git clone https://github.com/UoB-HPC/miniBUDE.git
WORKDIR miniBUDE
RUN git checkout db0570985356b3dafd1d675d44c9f89ee9e186a9 && git apply /root/roc-stdpar/data/patches/miniBUDE/MINIBUDE.patch
RUN cmake -B build -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCLANG_OFFLOAD=${OFFLOAD_ARCH} -DCLANG_STDPAR_PATH=/root/roc-stdpar/include -DMODEL=std-indices -DCMAKE_BUILD_TYPE=Release .
RUN cmake --build build -j$(nproc)

# nbody-stdpar
WORKDIR /root
RUN git clone https://github.com/jbeisman/nbody-stdpar.git
WORKDIR nbody-stdpar
RUN git checkout 3253e13d45222582ef3134c8177030b9837ba35d && git apply /root/roc-stdpar/data/patches/nbody-stdpar/NBODY_STDPAR.patch
RUN cmake -B build -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DUSE_OFFLOAD=ON -DSTDPAR_PATH=/root/roc-stdpar/include -DSTDPAR_TARGET=${OFFLOAD_ARCH} -DCMAKE_BUILD_TYPE=Release .
RUN cmake --build build -j$(nproc)


# p2r-tests
WORKDIR /root
RUN git clone https://github.com/cerati/p2r-tests.git
WORKDIR p2r-tests
RUN git checkout 88e88ab3814dfc535eba8c31970f985eff8681b8
#RUN git apply /root/roc-stdpar/data/patches/p2r-tests/P2R_TESTS.patch
# TODO

# palabos
WORKDIR /root
RUN git clone https://gitlab.com/unigehpfs/palabos.git
WORKDIR palabos
RUN git checkout 13bc5ba4d03306367ef65983aceaf0185a535c91 && git apply /root/roc-stdpar/data/patches/palabos/PALABOS.patch
WORKDIR examples/80days/tgv3d/build
RUN cmake -DCLANG_STDPAR=1 -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCMAKE_PREFIX_PATH=/opt/rocm-5.6.0/lib/cmake/ -DENABLE_MPI=OFF -DCMAKE_BUILD_TYPE=Release -DSTDPAR_PATH=/root/roc-stdpar/include -DSTDPAR_TARGET=${OFFLOAD_ARCH} ..
RUN make -j16
WORKDIR /root/palabos/examples/80days/rayleighTaylor3D/build
RUN cmake -DCLANG_STDPAR=1 -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCMAKE_PREFIX_PATH=/opt/rocm-5.6.0/lib/cmake/ -DENABLE_MPI=OFF -DCMAKE_BUILD_TYPE=Release -DSTDPAR_PATH=/root/roc-stdpar/include -DSTDPAR_TARGET=${OFFLOAD_ARCH} ..
RUN make -j16
WORKDIR /root/palabos/examples/80days/sandstone/build
RUN cmake -DCLANG_STDPAR=1 -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCMAKE_PREFIX_PATH=/opt/rocm-5.6.0/lib/cmake/ -DENABLE_MPI=OFF -DCMAKE_BUILD_TYPE=Release -DSTDPAR_PATH=/root/roc-stdpar/include -DSTDPAR_TARGET=${OFFLOAD_ARCH} ..
RUN make -j16
RUN wget -O Berea.ascii https://www.dropbox.com/s/6mf545fva4e7hf2/Berea.ascii?dl=0

# parSTL
#WORKDIR /root
#RUN git clone https://github.com/weilewei/parSTL.git
#WORKDIR parSTL
#RUN git checkout c385e2ee8359a5e046fa50e3e08a50e97a482285
#RUN git apply /root/roc-stdpar/data/patches/parSTL/PARSTL.patch
#RUN cmake -B build -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCLANG_OFFLOAD_GPU=${OFFLOAD_ARCH} -DCLANG_STDPAR_PATH=/root/roc-stdpar/include -DCMAKE_BUILD_TYPE=Release .
#RUN cmake --build build -j$(nproc)
# TODO

# paralg
WORKDIR /root
RUN git clone https://gitlab.com/unigehpfs/paralg.git
WORKDIR paralg
RUN git checkout 0450e72bd4081ce63a3fe4f42a3044709fa12b8c && git apply /root/roc-stdpar/data/patches/paralg/PARALG.patch
WORKDIR heatEquation
RUN make nompi_clang

# stdpar-mandelbrot
WORKDIR /root
RUN git clone https://github.com/UoB-HPC/stdpar-mandelbrot.git
WORKDIR stdpar-mandelbrot
RUN git checkout 93a3b9294a487bd66ff51671612c4a3d6e45c66b && git apply /root/roc-stdpar/data/patches/stdpar-mandelbrot/STDPAR_MANDELBROT.patch
RUN cmake -B build -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCLANG_OFFLOAD_GPU=${OFFLOAD_ARCH} -DCLANG_STDPAR_PATH=/root/roc-stdpar/include -DCMAKE_BUILD_TYPE=Release .
RUN cmake --build build -j$(nproc)

# stlbm
WORKDIR /root
RUN git clone https://gitlab.com/unigehpfs/stlbm.git
WORKDIR stlbm
RUN git checkout 326d7175ced0b6c84b9f1d574eebc66385de589e && git apply /root/roc-stdpar/data/patches/stlbm/STLBM.patch
RUN cmake -B build -DCMAKE_CXX_COMPILER=/opt/bin/clang++ -DCMAKE_PREFIX_PATH=/opt/rocm-5.6.0/lib/cmake/ -DCMAKE_BUILD_TYPE=Release -DSTDPAR_PATH=/root/roc-stdpar/include -DSTDPAR_TARGET=${OFFLOAD_ARCH} .
RUN cmake --build build -j$(nproc)

# traccc
#WORKDIR /root
#RUN git clone https://github.com/ugGit/traccc.git
#WORKDIR traccc
#RUN git checkout 6c6572f16fcf7e8136db85059becdcd8a0fcb604 && git apply /root/roc-stdpar/data/patches/traccc/TRACCC.patch
# TODO

# tsp
WORKDIR /root
RUN git clone https://github.com/pkestene/tsp.git
WORKDIR tsp
RUN git checkout 515872774fdeb5caf2ee13cbf1d64e3720e8995b
RUN patch -p1 < /root/roc-stdpar/data/patches/tsp/TSP.patch
WORKDIR stdpar
RUN make tsp_clang_stdpar_cpu
RUN make tsp_clang_stdpar_gpu

WORKDIR /root

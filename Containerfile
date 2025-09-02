FROM alpine:latest as bash_base
RUN apk update
RUN apk add --no-cache cmake make git llvm20 llvm20-dev ninja clang bash

FROM bash_base as final_build_layer
SHELL ["/bin/bash", "-c"]
WORKDIR /home/libs_build/musl
RUN rm -rf build/ && \
    chmod +x configure && \

    # Configure to build and replace the musl lib the container is using (we have little to no risk to do this since a container can be re-spun up if broken).
    ./configure --prefix=/usr/ --enable-optimize --disable-shared CC=clang && \
    make && \
    make install && \

    # Now rebuild libs but place them locally so we can access it locally outside of the container:
    ./configure --prefix=../musl_libc_build/ --enable-optimize --disable-shared CC=clang && \
    make && \
    make install

WORKDIR /home/libs_build/llvm-project
RUN rm -rf build/ && mkdir -p build && \
    cmake -G Ninja -S runtimes -B build -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" -DLIBCXX_HAS_MUSL_LIBC=On -DCMAKE_INSTALL_PREFIX=../llvm-libc++_build/ -DCMAKE_CXX_COMPILER="/usr/bin/clang++" -DCMAKE_C_COMPILER="/usr/bin/clang" && \
    ninja -C build cxx cxxabi unwind && \
    ninja -C build install-cxx install-cxxabi install-unwind

ENTRYPOINT ["/bin/bash"]
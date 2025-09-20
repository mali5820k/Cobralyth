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
    cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" -DLIBCXX_HAS_MUSL_LIBC=On -DCMAKE_CXX_COMPILER="/usr/bin/clang++" -DCMAKE_C_COMPILER="/usr/bin/clang" -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON&& \
    cmake --build build -j8 && \
    cmake --install build/ --prefix ../llvm_build/

ENTRYPOINT ["/bin/bash"]
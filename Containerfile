FROM docker.io/muslcc/x86_64:x86_64-linux-musl
RUN apk update
RUN apk add --no-cache cmake make git ninja python3 bash clang llvm llvm-dev libexecinfo libexecinfo-static libexecinfo-dev

SHELL ["/bin/bash", "-c"]
WORKDIR /home/libs_build/musl
RUN rm -rf build/ && \
    chmod +x configure && \
    # Configure to build and replace the musl lib the container is using (we have little to no risk to do this since a container can be re-spun up if broken).
    ./configure --prefix=/usr/ --enable-optimize --disable-shared && \
    make && \
    make install && \
    # Now rebuild libs but place them locally so we can access it locally outside of the container:
    ./configure --prefix=../musl_libc_build/ --enable-optimize --disable-shared && \
    make && \
    make install

# Copy out the installed libs for libc++ and musl-libc sysroot:
# WORKDIR /home/libs_build/
# RUN mkdir extracted_sysroot/ && \
#     cd extracted_sysroot/ && \
#     cp -rf /usr/include /usr/x86_64-alpine-linux-musl /usr/bin/clang /usr/bin/clang++ /usr/lib/ .
    
# WORKDIR /home/libs_build/llvm-project
# RUN rm -rf build/ && mkdir -p build && \
#     cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON  -DLLVM_ENABLE_PROJECTS="clang" && \
#     cmake --build build -j$(nproc) && \
#     cmake --install build/ --prefix ../llvm_build/
# WORKDIR /home/libs_build/llvm-project
# RUN rm -rf build/ && mkdir -p build && \
#     cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON  -DLLVM_ENABLE_RUNTIMES="libunwind" -DLIBCXXABI_USE_LLVM_UNWINDER=YES -DLIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY=ON -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=ON -DLIBCXX_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_ENABLE_EXCEPTIONS=ON -DLIBUNWIND_ENABLE_EXCEPTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_DEFAULT_TARGET_TRIPLE="x86_64-linux-musl" && \
#     cmake --build build -j$(nproc) && \
#     cmake --install build/ --prefix ../llvm_build/
# WORKDIR /home/libs_build/llvm-project
# RUN rm -rf build/ && mkdir -p build && \
#     cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON  -DLLVM_ENABLE_RUNTIMES="libcxxabi" -DLIBCXXABI_USE_LLVM_UNWINDER=YES -DLIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY=ON -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=ON -DLIBCXX_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_ENABLE_EXCEPTIONS=ON -DLIBUNWIND_ENABLE_EXCEPTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_DEFAULT_TARGET_TRIPLE="x86_64-linux-musl" && \
#     cmake --build build -j$(nproc) && \
#     cmake --install build/ --prefix ../llvm_build/
# WORKDIR /home/libs_build/llvm-project
# RUN rm -rf build/ && mkdir -p build && \
#     cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON  -DLLVM_ENABLE_RUNTIMES="libcxx" -DLIBCXXABI_USE_LLVM_UNWINDER=YES -DLIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY=ON -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=ON -DLIBCXX_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_ENABLE_EXCEPTIONS=ON -DLIBUNWIND_ENABLE_EXCEPTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_DEFAULT_TARGET_TRIPLE="x86_64-linux-musl" && \
#     cmake --build build -j$(nproc) && \
#     cmake --install build/ --prefix ../llvm_build/
# WORKDIR /home/libs_build/llvm-project
# RUN rm -rf build/ && mkdir -p build && \
#     cmake -S llvm -B build -sysroot=$CUSTOM_SYSROOT -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON  -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" -DLIBCXXABI_USE_LLVM_UNWINDER=YES -DLIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY=ON -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=ON -DLIBCXX_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_ENABLE_EXCEPTIONS=ON -DLIBUNWIND_ENABLE_EXCEPTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_DEFAULT_TARGET_TRIPLE="x86_64-linux-musl" && \
#     cmake --build build -j$(nproc) && \
#     cmake --install build/ --prefix ../llvm_build/

# WORKDIR /home/libs_build/llvm-project
# RUN rm -rf build/ && mkdir -p build && \
#     cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLIBCXX_HAS_MUSL_LIBC=ON -DLIBCXX_HAS_GCC_S_LIB=OFF -DCLANG_DEFAULT_CXX_STDLIB=libc++ -DCLANG_DEFAULT_LINKER=lld -DCLANG_DEFAULT_RTLIB=compiler-rt -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-pc-linux-musl -DLLVM_TARGET_ARCH=x86_64 -DLIBCXXABI_LIBCXX_PATH=/home/libs_build/llvm-project/libcxx -DLIBCXX_LIBCXX_INCLUDES_INTERNAL=/home/libs_build/llvm-project/libcxx/include -DLIBCXX_LIBCXXABI_INCLUDES_INTERNAL=/home/libs_build/llvm-project/libcxxabi/include -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON  -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_RUNTIMES="libcxxabi;libunwind;libcxx" -DLIBCXXABI_USE_LLVM_UNWINDER=YES -DLIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY=ON -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=ON -DLIBCXX_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_LIBCXX_PATH=/home/libs_build/llvm-project/ -DLIBUNWIND_ENABLE_EXCEPTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" && \ 
#     cmake --build build -j$(nproc) && \
#     cmake --install build/ --prefix ../llvm_build/
WORKDIR /home/libs_build/llvm-project
RUN cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLIBCXX_HAS_MUSL_LIBC=ON -DLIBCXX_HAS_GCC_S_LIB=OFF -DCLANG_DEFAULT_CXX_STDLIB=libc++ -DCLANG_DEFAULT_LINKER=lld -DCLANG_DEFAULT_RTLIB=compiler-rt -DLLVM_ENABLE_RUNTIMES="libunwind;libcxxabi;libcxx" -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-pc-linux-musl -DLLVM_TARGET_ARCH=x86_64 -DLIBCXXABI_LIBCXX_PATH=/home/libs_build/llvm-project/libcxx -DLIBCXX_LIBCXX_INCLUDES_INTERNAL=/home/libs_build/llvm-project/libcxx/include -DLIBCXX_LIBCXXABI_INCLUDES_INTERNAL=/home/libs_build/llvm-project/libcxxabi/include -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON -DLIBCXXABI_USE_LLVM_UNWINDER=YES -DLIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY=ON -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=ON -DLIBCXX_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_LIBCXX_PATH=/home/libs_build/llvm-project/ -DLIBUNWIND_ENABLE_EXCEPTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" && \ 
    cmake --build build -j$(nproc) && \
    cmake --install build/ --prefix ../llvm_build/


RUN cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLIBCXX_HAS_MUSL_LIBC=ON -DLIBCXX_HAS_GCC_S_LIB=OFF -DCLANG_DEFAULT_CXX_STDLIB=libc++ -DCLANG_DEFAULT_LINKER=lld -DCLANG_DEFAULT_RTLIB=compiler-rt -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-pc-linux-musl -DLLVM_TARGET_ARCH=x86_64 -DLIBCXXABI_LIBCXX_PATH=/home/libs_build/llvm-project/libcxx -DLIBCXX_LIBCXX_INCLUDES_INTERNAL=/home/libs_build/llvm-project/libcxx/include -DLIBCXX_LIBCXXABI_INCLUDES_INTERNAL=/home/libs_build/llvm-project/libcxxabi/include -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_ENABLE_PROJECTS="clang" -DLIBCXXABI_USE_LLVM_UNWINDER=YES -DLIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY=ON -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=ON -DLIBCXX_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_LIBCXX_PATH=/home/libs_build/llvm-project -DLIBUNWIND_ENABLE_EXCEPTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" && \ 
    cmake --build build -j$(nproc) && \
    cmake --install build/ --prefix ../llvm_build/
    # -DLLVM_DEFAULT_TARGET_TRIPLE="x86_64-linux-musl" && \

    # Need to make a mega-archive with the musl libs:
    #ar x /usr/lib/musl
#RUN rm -rf build/ && mkdir -p build && \
#    cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ASSERTIONS=ON  -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" -DLIBCXX_HAS_MUSL_LIBC=ON -DLIBCXXABI_USE_LLVM_UNWINDER=YES -DLIBCXX_STATICALLY_LINK_ABI_IN_STATIC_LIBRARY=ON -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=ON -DLIBCXX_ENABLE_EXCEPTIONS=ON -DLIBCXXABI_ENABLE_EXCEPTIONS=ON -DLIBUNWIND_ENABLE_EXCEPTIONS=ON && \
#    cmake --build build -j$(nproc) && \
#    cmake --install build/ --prefix ../llvm_build/ && \
#    # Need to make a mega-archive with the musl libs:
#    ar x /usr/lib/musl

ENTRYPOINT ["/bin/bash"]

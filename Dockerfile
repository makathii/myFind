#
# \file Dockerfile
# \author Felix Dilly
# \date Created at: 2025-06-07
# \date Last modified at: 2025-06-07
# ---
#

FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

RUN sudo apt update &&\
    sudo apt install -y \
    clang-format


#
# CMake
#
#RUN wget https://github.com/Kitware/CMake/archive/refs/tags/v4.0.2.tar.gz -O cmake.tar.gz && \
#    tar xf cmake.tar.gz && \
#    rm cmake.tar.gz && \
#    cd CMake-* && \
#    ./bootstrap --prefix=/usr --no-qt-gui --parallel=$(nproc) && \
#    make -j$(nproc) && \
#    make install && \
#    cd .. && \
#    rm -rf CMake*

#
# clang language server
#
RUN wget https://github.com/clangd/clangd/releases/download/20.1.0/clangd-linux-20.1.0.zip && \
    unzip clangd-linux-20.1.0.zip && \
    mkdir /usr/local/bin/clang-20 && \
    cp /clangd_20.1.0/bin/clangd /usr/local/bin/clangd-20 &&\
    update-alternatives --install /usr/bin/clangd clangd /usr/local/bin/clangd-20 100

#
# Catch2 unit test framework
#
RUN git clone --recursive -b v3.8.1 --depth 1 https://github.com/catchorg/Catch2.git /catch2 && \
    cd catch2 && \
    cmake -Bbuild -H. -DBUILD_TESTING=OFF && \
    cmake --build build/ --target install && \
    cd / && \
    rm -rf /catch2

#
# spd
#
RUN git clone --recursive -b v1.15.3 --depth 1 https://github.com/gabime/spdlog.git /spdlog && \
    cd /spdlog && \
    cmake -S . && \
    make install && \
    cd / && \
    rm -rf /spdlog


RUN ldconfig

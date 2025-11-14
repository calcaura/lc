FROM ubuntu:24.04

# Update indexes
RUN apt-get update && apt-get install -y \
    ca-certificates \
    build-essential \
    clang \
    clang-tools \
    clang-tidy \
    clang-format \
    cmake \
    curl \
    gdb \
    lldb \
    git \
    ninja-build \
    sudo \
    vim \
    libgtest-dev \
    man-db manpages manpages-posix manpages-dev

# Build GTest
RUN cd /usr/src/gtest && \
    cmake . &&\
    make && \
    cp lib/*.a /usr/lib && \
    mkdir /usr/local/lib/googletest && \
    ln -s /usr/lib/libgtest.a /usr/local/lib/googletest/libgtest.a && \
    ln -s /usr/lib/libgtest_main.a /usr/local/lib/googletest

## Install abseil 20250814.1
ENV ABSEIL_VERSION=20250814.1
RUN curl -Lo abseil-cpp-${ABSEIL_VERSION}.tar.gz \
    https://github.com/abseil/abseil-cpp/releases/download/${ABSEIL_VERSION}/abseil-cpp-${ABSEIL_VERSION}.tar.gz && \
    tar -xzf abseil-cpp-${ABSEIL_VERSION}.tar.gz -C /usr/src/ && \
    rm abseil-cpp-${ABSEIL_VERSION}.tar.gz && \
    cd /usr/src/abseil-cpp-${ABSEIL_VERSION} && \
    mkdir build && \
    cd build && \
    cmake -S .. -B . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON &&\
    cmake --build . -j$(nproc) && \
    sudo cmake --install . --prefix /usr/local


## Install Google Benchmark v1.9.4
ENV BENCHMARK_VERSION=v1.9.4
RUN curl -Lo benchmark-1.9.4.tar.gz \
    https://github.com/google/benchmark/archive/refs/tags/v1.9.4.tar.gz && \
    tar -xzf benchmark-1.9.4.tar.gz -C /usr/src/ && \
    rm benchmark-1.9.4.tar.gz && \
    cd /usr/src/benchmark-1.9.4 && \
    mkdir build && \
    cd build && \
    cmake -S .. -B . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON \
    -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON && \
    cmake --build . -j$(nproc) && \
    sudo cmake --install . --prefix /usr/local


## Configure default user
ARG HOST_USER
ENV REMOTE_USER=${HOST_USER:-lcdev}

ARG HOST_UID
ENV REMOTE_UID=${HOST_UID:-1000}

ARG HOST_GID
ENV REMOTE_GID=${HOST_GID:-1000}

## Default EDITOR
ENV EDITOR=vim

ENV LC_TOOLS=/opt/lc
RUN mkdir -p ${LC_TOOLS}
ADD .devcontainer/artifacts ${LC_TOOLS}
RUN cat ${LC_TOOLS}/bashrc_ext.sh >> /etc/bash.bashrc

## Add the current user mapped to the ids of the local git repository user
RUN userdel -fr ubuntu || true
RUN groupadd --gid ${REMOTE_GID} ${REMOTE_USER} || true
RUN useradd -m --uid ${REMOTE_UID} --gid ${REMOTE_GID} ${REMOTE_USER} -s /bin/bash
RUN usermod -a -G sudo ${REMOTE_USER}
RUN passwd -d ${REMOTE_USER}

# Uniminimize in order to get all manpages
RUN yes | unminimize

# Cleanup the container
RUN rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
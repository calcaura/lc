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
    sudo \
    vim \
    libgtest-dev

# Build GTest
RUN cd /usr/src/gtest && \
    cmake . &&\
    make && \
    cp lib/*.a /usr/lib && \
    mkdir /usr/local/lib/googletest && \
    ln -s /usr/lib/libgtest.a /usr/local/lib/googletest/libgtest.a && \
    ln -s /usr/lib/libgtest_main.a /usr/local/lib/googletest

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
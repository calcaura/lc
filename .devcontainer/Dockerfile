FROM alpine:latest

RUN echo "https://dl-cdn.alpinelinux.org/alpine/edge/community" >> /etc/apk/repositories

# Update indexes
RUN apk update && apk add \
    bash \
    build-base \
    coreutils \
    clang \
    clang-extra-tools \
    curl \
    git \
    github-cli \
    gdb \
    lld \
    lldb \
    make \
    openssh-client \
    sudo \
    vim \
    tar

# Configure default user
ARG HOST_USER
ENV REMOTE_USER=${HOST_USER:-lcdev}

# Default EDITOR
ENV EDITOR=vim

ENV LC_TOOLS=/opt/lc
RUN mkdir -p ${LC_TOOLS}
ADD .devcontainer/artifacts ${LC_TOOLS}


# Add the current user mapped to the ids of the local git repository user
RUN adduser -D ${REMOTE_USER}  -s /bin/bash
RUN adduser $REMOTE_USER wheel
RUN echo "$REMOTE_USER ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$REMOTE_USER && chmod 0440 /etc/sudoers.d/$REMOTE_USER
RUN echo "wheel ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/wheel

RUN echo "PS1='\[\e[1;32m\]\u@\h \[\e[1;34m\]\w \$\[\e[0m\] '" >> /etc/bash/bashrc
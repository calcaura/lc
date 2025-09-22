#!/bin/bash

exec 2>&1 1> /tmp/lc-post-create.log

HOST_MOUNT_PATH=$HOME/host
FILES_TO_SYMLINK=(
    .bashrc
    .bash_aliases
    .bash_history
    .bashrc_devcontainer
    .gitconfig
    .ssh
    .vim
    .vimrc
)

# Fixing the workspace access
git config --global --add safe.directory /workspace

# Unlinking the .ssh folder if it's not a symlink
SSH_FOLDER="$HOME/.ssh"
if [[ -d "${SSH_FOLDER}" && ! -L "${SSH_FOLDER}" ]]; then
    rm -f "${SSH_FOLDER}/known_hosts"
    if [[ -z "$(ls -A ${SSH_FOLDER})" ]]; then
        echo "Unlinking the ${SSH_FOLDER} file"
        rm -fr "${SSH_FOLDER}"
    fi
fi

# Symlinking 
for file in ${FILES_TO_SYMLINK[@]}; do
    what=$HOME/host/$file
    to=$HOME/$file
    if [[ -f $what || -d $what ]]; then
        ln -sf $what $to || echo "Failed to symlink $what -> $to"
    else
        echo "File $what doesn't exist"
    fi
done

# Fix hostname
expected_host=$(hostname)
grep ${expected_host} /etc/hosts || (echo "127.0.0.1 ${expected_host}" | sudo tee -a /etc/hosts)

[ -f $HOME/.bash_aliases ] && echo "source $HOME/.bash_aliases" >> $HOME/.bashrc

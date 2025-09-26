#!/bin/bash
set -e

cd "$(dirname "$0")"
DIRS=$(find -name 'Makefile' | grep -v _template | xargs dirname)

__date() {
  date +'%Y-%m-%dT%H:%M:%S%z'
}

log() {
  printf "\e[32m[%s]: %s\e[0m\n" "$(__date)" "$1"
}

info() {
  printf "\e[33m[%s]: %s\e[0m\n" "$(__date)" "$1"
}


error() {
  >&2 printf "\e[31m[%s]: %s\e[0m\n" "$(__date)" "$1"
}

clear_line() {
  # terminfo clr_eol
  ceol=$(tput el)
  echo -ne "\r${ceol}\033[2K"
}


die_with() {
  error "$1"
  exit 1
}

function build_single() {
    local dir="$1"
    local title=$(head -1 "$dir/README.md" | sed 's/^#* //g')
    sync
    echo "::group::$title ($dir)"
    make -C "$dir"
    printf "::endgroup::\n\n"
    sync
}

export CXX_FLAGS_EXTRA="-g -O0 -fsanitize=address -fsanitize=undefined"

for dir in $DIRS; do
    build_single "$dir"
done
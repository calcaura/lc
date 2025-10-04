#!/bin/bash
cd "$(dirname "$0")" || exit 1
DIRS=$(find -name 'Makefile' | grep -v _template | xargs dirname | sort -u)
TMP_DIR=$(mktemp -d)

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
    local name=$(basename $dir)
    local OUTPUT=${TMP_DIR}/${name}.out
    local ERR=${TMP_DIR}/${name}.err

    info "Starting test ${name}"
    
    echo "::group::$title ($dir)" > ${OUTPUT}
    make -C "$dir" >> ${OUTPUT} 2> ${ERR}
    printf "::endgroup::\n\n" >> ${OUTPUT}

    local err=$?
    local err_out=$(cat ${ERR})
    if [ ! -z "${err_out}" ] || [ $err -ne 0 ] || grep -q FAILED ${OUTPUT}; then
      echo "::error::${title} ($dir)::" >> ${OUTPUT} 2>&1
    fi

    info "Done test ${name}"
    
}
N=$(cat /proc/cpuinfo | grep processor | wc -l)   # max parallel jobs
queue=()

run_job() {
    local cmd="$1"
    build_single "$cmd" &
    queue+=($!)
}

check_queue() {
    local new_queue=()
    for pid in "${queue[@]}"; do
        if kill -0 "$pid" 2>/dev/null; then
            new_queue+=($pid)
        fi
    done
    queue=("${new_queue[@]}")
}


log "Output under ${TMP_DIR}"

# Starting all tests
export CXX_FLAGS_EXTRA="-g -O0 -fsanitize=address -fsanitize=undefined"
for dir in $DIRS; do
    check_queue
    while [ "${#queue[@]}" -ge "$N" ]; do
        sleep 0.5
        check_queue
    done
    run_job "$dir"
done

# wait for remaining
wait

log "All tests done"

errors_out=$(cat ${TMP_DIR}/*.err)
if [ ! -z "${errors_out}" ]; then
  sync
  cat ${TMP_DIR}/*.out
  sync
  die_with "${errors_out}"
fi

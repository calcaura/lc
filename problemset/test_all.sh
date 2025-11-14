#!/bin/bash
cd "$(dirname "$0")" || exit 1
source ../libs/bash/console.sh || exit 2

DIRS=$(find -name 'Makefile' -not -path '*/build/*' | grep -v _template | xargs dirname | sort -u)
TMP_DIR=$(mktemp -d)


function build_single() {
    local dir="$1"
    local cmd="$2"

    local title=$(head -1 "$dir/README.md" | sed 's/^#* //g')
    local name=$(basename $dir)
    local OUTPUT=${TMP_DIR}/${name}.out
    local ERR=${TMP_DIR}/${name}.err

    log_info ">>> Begin $cmd ${name}"
    
    echo "::group::$title ($dir)" > ${OUTPUT}
    make -C "$dir" $cmd >> ${OUTPUT} 2> ${ERR}
    printf "::endgroup::\n\n" >> ${OUTPUT}

    local err=$?
    local err_out=$(cat ${ERR})
    if [ ! -z "${err_out}" ] || [ $err -ne 0 ] || grep -q FAILED ${OUTPUT}; then
      echo "::error::${title} ($dir)::" >> ${OUTPUT} 2>&1
    fi

    log_info "<<<   End $cmd ${name}"
    
}
N=$(cat /proc/cpuinfo | grep processor | wc -l)   # max parallel jobs
queue=()

run_job() {
    local dir="$1"
    local cmd="$2"
    build_single "$dir" "$cmd" &
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

run_make_target() {
  local target=$1
  log_debug "Running target $target"
  for dir in $DIRS; do
      check_queue
      while [ "${#queue[@]}" -ge "$N" ]; do
          sleep 0.2
          check_queue
      done
      run_job "$dir" "$target"
  done

  # wait for remaining
  wait

  # check for errors
  errors_out=$(cat ${TMP_DIR}/*.err)
  if [ ! -z "${errors_out}" ]; then
    log_error "While executing target $target"
  fi
}

log_debug "Output under ${TMP_DIR}"

# Starting all tests
export CXX_FLAGS_EXTRA="-g -O0 -fsanitize=address -fsanitize=undefined"

for target in tidy build test; do
  run_make_target $target || break
done

log_debug "All tests done"

errors_out=$(cat ${TMP_DIR}/*.err)
if [ ! -z "${errors_out}" ]; then
  sync
  cat ${TMP_DIR}/*.out
  sync
  die_with "${errors_out}"
fi

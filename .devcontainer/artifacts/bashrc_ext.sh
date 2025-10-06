
__date() {
  date +'%Y-%m-%dT%H:%M:%S%z'
}

log_debug() {
  printf "\e[32m[%s]: %s\e[0m\n" "$(__date)" "$1"
}

log_info() {
  printf "\e[33m[%s]: %s\e[0m\n" "$(__date)" "$1"
}


log_error() {
  >&2 printf "\e[31m[%s]: %s\e[0m\n" "$(__date)" "$1"
  false
}

git-restack() {
    local file_list=$(git status --porcelain) || return
    if [ ! -z "$file_list" ] ; then
       log_error "Modified files found. Please commit, revert or stash"
       return
    fi
    local crt_branch=$(git branch --show-current)
    [ "$crt_branch" == "main" ] || git checkout main
    log_debug "Pulling main ..."
    git pull -p || return

    local branch_list=$(git branch --format="%(refname:short)" | grep -v main)

    log_debug "Restacking branches: ($branch_list)"

    for branch in $branch_list; do
      log_debug "Restacking $branch"
      git checkout $branch || return
      git fetch --prune

      if ! git rev-parse --abbrev-ref --symbolic-full-name @{u} >/dev/null 2>&1; then
          log_error "⚠️ Upstream is gone for $branch (use git-gc to garbage-collect)"
          continue
      fi
      
      GIT_EDITOR=true git rebase -i  --autosquash main --autostash || return
      log_info "✅ Branch $branch restacked"
    done

    [ "$crt_branch" == "main" ] || git checkout $crt_branch
}

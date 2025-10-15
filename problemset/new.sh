#!/bin/bash
cd "$(dirname "$0")" || exit 1
source ../libs/bash/console.sh || exit 2

usage() {
  [ -z "$1" ] || log_error $1

  die_with "Usage:
  ./new.sh xx. <title> [source]

  Where:
    xx is the problem number, e.g. 61
    <title> is the problem title, e.g. Rotate List
    [source] is an optional argument to specify the source of the problem, e.g. leetcode, hackerrank, etc. Default is leetcode.
  
  Example:
    ./new.sh "61. Rotate List"

  Note: The title should be obtained by simply right-click+copy on the title in leetcode.
  "
}

parse_title() {
  local input="$1"
  local source="${2:-leetcode}"

  [[ $input =~ ^([0-9]+)\.\ (.*)$ ]] || usage
  local num="${BASH_REMATCH[1]}"
  local rest="${BASH_REMATCH[2]}"
  [ -z "$num" ] && usage "Name expected to start with a number"
  [ -z "$rest" ] && usage "The remaining of the title exepcted to be non empty"

  local proj_name="${rest// /}"
  local proj_name="${proj_name//[()]/_}"

  export PROJECT_NUMBER=$(printf "%04d" "$num")
  export PROJECT_NAME="${PROJECT_NUMBER}.${proj_name}"
  export PROJECT_TITLE=$rest
  export PROJECT_TC_TITLE=$(echo "$rest" | tr -d ' ')
  export PROJECT_GIT_BRANCH="$(echo "$PROJECT_NAME" | tr '[:upper:]' '[:lower:]')"
  export PROJECT_DIR="${source}/${PROJECT_NAME}"
}

parse_title "$1" $2 || exit 1

[ -d $PROJECT_NAME ] && die_with "Project already exists: $PROJECT_NAME"
git show-ref --verify --quiet "refs/heads/$PROJECT_GIT_BRANCH" && die_with "Git branch already exists: ${PROJECT_GIT_BRANCH}"

log_debug "Copying the template into $PROJECT_DIR"
cp -r common/_template "$PROJECT_DIR"  || exit 1

log_debug "Updating the title in README.md"
echo "# ${PROJECT_TITLE}" >> "$PROJECT_DIR/README.md"  || exit 1

log_debug "Updating test case"
sed -i s/TemplateNameTest/${PROJECT_TC_TITLE}/g "$PROJECT_DIR/test.cpp"  || exit 1

log_debug "Creating git branch '$PROJECT_GIT_BRANCH'"
git checkout -b ${PROJECT_GIT_BRANCH} || exit 1

log_debug "Adding files"
git add ${PROJECT_NAME} || exit 1

log_debug "Initial commit"
GIT_EDITOR=true git commit -m "$PROJECT_TITLE"

log_info "✅ Project ready: $PROJECT_NAME"
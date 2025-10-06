# Source-able file

__date() {
  date +'%Y-%m-%dT%H:%M:%S%z'
}

# For more colors, see https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences

_COLOR_BLUE=34
_COLOR_GREEN=32
_COLOR_YELLOW=33
_COLOR_RED=31


_log_colored() {
    local color=$1
    local level=$2
    shift
    shift # gettting all args till the end
    local msg=$@
    if [ -t 1 ]; then
        printf "\e[${color}m[%s][%s]: %s\e[0m\n" "$(__date)" "$level"  "$msg"
    else
        printf "[%s][%s]: %s\n" "$(__date)" "$level"  "$msg"
    fi
}

log_debug() {
  _log_colored $_COLOR_BLUE debug $@
}

log_info() {
  _log_colored $_COLOR_GREEN info $@
}

log_warn() {
  _log_colored $_COLOR_YELLOW warn $@
}

log_error() {
  >&2 _log_colored $_COLOR_RED error $@
  # Also, make sure that the last return code is set to error
  false
}

clear_line() {
  # terminfo clr_eol
  ceol=$(tput el)
  echo -ne "\r${ceol}\033[2K"
}


die_with() {
  log_error "$1"
  exit 1
}
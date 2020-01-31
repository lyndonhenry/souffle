#!/bin/bash

set -oue pipefail

function error() {
    local MESSAGE="${1:-}"
    if [ "${MESSAGE}" ]
    then
        echo "Error: ${MESSAGE}" >&2
    fi
    exit 1
}

function clang_format_hook() {
    local COMMAND="clang-format"
    local VERSION="7"
    if [ ! $(which "${COMMAND}") ]
    then
        error "'${COMMAND}' is not installed!"
    fi
    if [ "$(${COMMAND} --version | sed 's/.*version //;s/\..*//')" -lt "${VERSION}" ]
    then
        error "'${COMMAND}' is not of version '${VERSION}' or later!"
    fi
    ${COMMAND} \
        -i \
        -style=file \
        src/*.h \
        src/*.cpp \
        src/test/*.h \
        src/test/*.cpp \
        tests/interface/*/*.cpp
}

function main() {
    clang_format_hook
}

main "${@:-}"

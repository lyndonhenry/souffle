#!/bin/bash

set -oue pipefail

function clang_format_hook() {
    local EXE="clang-format"
    local VERSION="7"
    if [ ! $(which ${EXE}) ]
    then
        EXE="${EXE}-${VERSION}"
        if [ ! $(which ${EXE}) ]
        then
            echo "Error: program '${EXE}' not found!"
            exit 1
        fi
    fi

    if [ "$(${EXE} --version | sed 's/.*version //;s/\..*//')" -lt "${VERSION}" ]
    then
        echo "Error: program '${EXE}' must be version ${VERSION} or later!"
        exit 1
    fi

    ${EXE} \
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

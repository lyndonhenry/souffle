#!/bin/bash

set -oue pipefail

if [ $(which clang-format-7) ]
then
    clang-format-7 \
            -i \
            -style=file \
            src/*.h \
            src/*.cpp \
            src/test/*.h \
            src/test/*.cpp \
            tests/interface/*/*.cpp
fi

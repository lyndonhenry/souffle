#!/bin/bash

set -e -u -o pipefail

if [ $(which clang-format-4.0) ]
then
    clang-format-4.0 \
            -i \
            -style=file \
            ./src/*.c \
            ./src/*.cc \
            ./src/*.cpp \
            ./src/*.h \
            ./src/*.hh \
            ./src/*.hpp
fi

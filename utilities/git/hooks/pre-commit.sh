#!/bin/bash

set -e -u -o pipefail

if [ $(which clang-format-4.0) ]
then
    clang-format-4.0 \
            -i \
            -style=file \
            $(find . -name *.h) \
            $(find . -name *.cpp)
fi

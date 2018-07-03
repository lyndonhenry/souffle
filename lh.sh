#!/bin/bash

# @TODO
# - put most of Mpi.h in an Mpi.cpp, too much in header
# - do not use anonymous namespaces in Mpi.h, maybe ok in cpp file
# - do documentation for whole of Mpi.h class
# - get it working with zero arity predicates
# - do debugging macros
# - must ensure that sends and receives are in the same order, they have to sync up
# - get travis tests working
# - ensure that removing mpi output works in testsuite.at for .out files
# - merge with upstream

set -x

function main() {
    if [ $# == 0 ]
    then
        ./utilities/git/hooks/pre-commit.sh
        export SOUFFLE_CONFS="-c -j8 -empi"
        ./bootstrap
        ./configure --enable-debug --enable-mpi
        make check -j8
    else
        local cwd=./tests/testsuite.dir/${1}-${2}
        mkdir -p ./tests/testsuite.dir
        cp -r ./tests/${1}/${2} ${cwd}
        ./src/souffle -empi -j8 -g${cwd}/${2}.cpp -D${cwd} -F${cwd}/facts -r${cwd}/${2}.html ${cwd}/${2}.dl
        clang-format-4.0 -i ${cwd}/${2}.cpp
        ./src/souffle -empi -j8 -c -D${cwd} -F${cwd}/facts ${cwd}/${2}.dl
    fi
}

main ${@}


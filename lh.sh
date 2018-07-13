#!/bin/bash

# @TODO

set -eoux pipefail

function main() {
    if [ $# == 0 ]
    then
        echo $(./utilities/git/hooks/pre-commit.sh)
        export SOUFFLE_CONFS="-c -j8 -empi"
        ./bootstrap
        ./configure --enable-debug --enable-mpi
        make -j8
    elif [ $# == 1 ]
    then
        export CC=gcc
        export CXX=g++
        export SOUFFLE_DOCKER_BASE_IMAGE=souffle/ubuntu:bionic
        export SOUFFLE_CATEGORY=Unit,Syntactic,Semantic,Profile,Evaluation
        export SOUFFLE_CONFIGURE_OPTIONS="--enable-mpi"
        export SOUFFLE_CONFS="-c -j8 -empi"
        export SOUFFLE_MAKE_JOBS=2
        ./.travis/run_docker.sh
    elif [ $# == 2 ]
    then
        local cwd=./tests/testsuite.dir/${1}-${2}
        rm -rf ${cwd}
        mkdir -p ./tests/testsuite.dir
        cp -r ./tests/${1}/${2} ${cwd}
        ./src/souffle -empi -j8 -g${cwd}/${2}.cpp -D${cwd} -F${cwd}/facts -r${cwd}/${2}.html ${cwd}/${2}.dl
        clang-format-4.0 -i ${cwd}/${2}.cpp
        ./src/souffle -empi -j8 -c -D${cwd} -F${cwd}/facts ${cwd}/${2}.dl
    fi
}

main ${@}

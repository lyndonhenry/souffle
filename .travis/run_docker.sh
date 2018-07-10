#!/bin/bash

set -ouex pipefail

if [ $# == 0 ];
then
    docker run \
        --volume $(pwd):/home/souffle/souffle \
        --env "CC=${CC}" \
        --env "CXX=${CXX}" \
        --env "SOUFFLE_CONFS=${SOUFFLE_CONFS}" \
        --env "SOUFFLE_CATEGORY=${SOUFFLE_CATEGORY}" \
        --env "SOUFFLE_CONFIGURE_OPTIONS=${SOUFFLE_CONFIGURE_OPTIONS}" \
        --env "SOUFFLE_MAKE_JOBS=${SOUFFLE_MAKE_JOBS}" \
        souffle/ubuntu:bionic-base \
        /bin/bash -c "cd souffle && ./.travis/run_docker.sh souffle"
else
    ulimit -c unlimited -S
    ./bootstrap
    ./configure ${SOUFFLE_CONFIGURE_OPTIONS}
    make -j${SOUFFLE_MAKE_JOBS}
    if [[ "$SOUFFLE_CATEGORY" != *"Unit"* ]]
    then
      cd tests
    fi
    TESTSUITEFLAGS="-j${SOUFFLE_MAKE_JOBS}" make check -j${SOUFFLE_MAKE_JOBS}
fi

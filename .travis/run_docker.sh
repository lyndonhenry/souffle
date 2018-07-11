#!/bin/bash

set -ouex pipefail

CC="gcc-7"
CXX="g++-7"

# CC=clang-4.0
# CXX=clang++-4.0

SOUFFLE_DOCKER_BASE_IMAGE="souffle/ubuntu:bionic-base"
SOUFFLE_CATEGORY="FastEvaluation,Interface,Profile,Provenance"
SOUFFLE_CONFIGURE_OPTIONS="--enable-mpi"
SOUFFLE_CONFS="-c -j8 -empi"
SOUFFLE_MAKE_JOBS="2"

SOUFFLE_DOCKER_TAG="$(echo ${SOUFFLE_DOCKER_BASE_IMAGE} | sed 's/-base/-test/g')"

docker build \
    --tag ${SOUFFLE_DOCKER_TAG} \
    --build-arg BASE_IMAGE="${SOUFFLE_DOCKER_BASE_IMAGE}" \
    --build-arg CC="${CC}" \
    --build-arg CXX="${CXX}" \
    --build-arg SOUFFLE_CONFS="${SOUFFLE_CONFS}" \
    --build-arg SOUFFLE_CATEGORY="${SOUFFLE_CATEGORY}" \
    --build-arg SOUFFLE_CONFIGURE_OPTIONS="${SOUFFLE_CONFIGURE_OPTIONS}" \
    --build-arg SOUFFLE_MAKE_JOBS="${SOUFFLE_MAKE_JOBS}"

docker run \
    --interactive \
    --tty \
    ${SOUFFLE_DOCKER_TAG}

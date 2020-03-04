#!/bin/bash

set -o -u -e pipefail

function docker_build() {
  docker build \
    --rm \
    --tag ${USER}/souffle \
    --file Dockerfile \
    --build-arg DOCKER_USER=${USER} \
    .
}
    
function docker_run() {
  docker run \
    --interactive \
    --tty \
    --volume ${PWD}:/home/${USER}/souffle \
    --volume /var/run/docker.sock:/var/run/docker.sock \
    ${USER}/souffle:latest
}

function main() {
  docker_build 
  docker_run
}

main ${@:-}



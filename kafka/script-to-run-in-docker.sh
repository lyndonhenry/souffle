#!/bin/bash

set -ouex pipefail

source ./kafka/.env

function _main() {

  # pull the latest from Dockerhub
  sudo docker pull ${IMAGE_NAME}

  # run the docker container, mounting the appropriate directories
  sudo docker run \
    --rm \
    --interactive \
    --tty \
    --net=host \
    --volume "/var/run/docker.sock:/var/run/docker.sock" \
    --volume "${HOME}/.souffle:/home/default/.souffle" \
    --volume "${HOME}/.aws:/home/default/.aws" \
    "${IMAGE_NAME}" \
    /bin/bash
}

_main
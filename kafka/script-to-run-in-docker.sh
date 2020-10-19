#!/bin/bash

set -ouex pipefail

function _main() {

  # pull the latest from Dockerhub
  sudo docker pull lyndonmhenry/souffle-on-kafka

  # run the docker container, mounting the appropriate directories
  sudo docker run \
    --rm \
    --interactive \
    --tty \
    --net=host \
    --volume "/var/run/docker.sock:/var/run/docker.sock" \
    --volume "${HOME}/.souffle:/home/default/.souffle" \
    --volume "${HOME}/.aws:/home/default/.aws" \
    "lyndonmhenry/souffle-on-kafka:latest" \
    /bin/bash
}

_main
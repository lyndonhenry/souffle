#!/bin/bash

set -ouex pipefail

#
# == Global Variables ===
#

source ./kafka/.env

#
# == Utility Functions =
#

function _error() {
  echo "Error!"
  exit 1
}

#
# == Docker Commands ==
#

#
# This deletes all Docker containers and images.
#
function _docker_clean() {
  sudo docker stop $(sudo docker ps -a -q) || :
  sudo docker rm $(sudo docker ps -a -q) || :
  sudo docker rmi $(sudo docker images -q -a) || :
  #sudo docker system prune -a
}

#
# Build a Souffle on Kafka Docker image.
#
function _docker_build() {
  sudo docker build \
  --rm \
  --tag "${IMAGE_NAME}" \
  --file "${DOCKER_FILE}" \
  --build-arg DOCKER_USER="${DOCKER_USER}" \
  --build-arg GITHUB_USER="${GITHUB_USER}" \
  --build-arg GITHUB_BRANCH="${GITHUB_BRANCH}" \
  "$(mktemp -d)"
}

#
# Run a Souffle on Kafka Docker container.
#
function _docker_run() {
  local FROM_DIR="${HOME}/.souffle"
  local TO_DIR="/home/${DOCKER_USER}/.souffle"
  mkdir -p "${FROM_DIR}"
  sudo docker run \
  --rm \
  --interactive \
  --tty \
  --net=host \
  --volume "/var/run/docker.sock:/var/run/docker.sock" \
  --volume "${FROM_DIR}:${TO_DIR}" \
  --volume "${HOME}/.aws:/home/${DOCKER_USER}/.aws" \
  "${IMAGE_NAME}:latest" \
  "${@:-}"
}

#
# Push the Souffle on Kafka Docker image to Dockerhub.
#

function _docker_push() {
  docker push ${IMAGE_NAME}:latest
}

#
# Run the Souffle testsuite inside of a Docker container
#

function _docker_test() {
  # ensure that souffle kafka is on path, assume ${PWD} is souffle home
  export PATH="${PWD}/kafka:${PATH}"
  local OLDPWD="${PWD}"
  local SOUFFLE_CATEGORY="FastEvaluation"
  local JOBS="2"
  local SC=""
  # all tests are in parallel
  SC+=" -j${JOBS} "
  local SOUFFLE_CONFS=""
  # interpreter
  SOUFFLE_CONFS+="${SC}"
  # compiler
  SOUFFLE_CONFS+=",${SC} -c"
  # file engine, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-file"
  # general strategy, interpreter
  SOUFFLE_CONFS+=",${SC} -Xuse-general"
  # general strategy, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-general"
  # general strategy, file engine, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-file -Xuse-general"
  # general producers strategy, interpreter
  SOUFFLE_CONFS+=",${SC} -Xuse-general -Xuse-general-producers"
  # general producers strategy, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-general -Xuse-general-producers"
  # general producers strategy, file engine, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-file -Xuse-general -Xuse-general-producers"
  # kafka engine, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-kafka"
  # kafka engine, general strategy, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-kafka -Xuse-general"
  # kafka engine, general producers strategy, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-kafka -Xuse-general -Xuse-general-producers"
  # kafka engine, general consumers strategy, compiler
  SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-kafka -Xuse-general -Xuse-general-producers -Xuse-general-consumers"
  export SOUFFLE_CATEGORY="${SOUFFLE_CATEGORY}"
  export SOUFFLE_CONFS="${SOUFFLE_CONFS}"
  # run souffle testsuite with relevant cases
  cd kafka
  sudo docker-compose up -d --remove-orphans
  cd ..
  TESTSUITEFLAGS="-j${JOBS}" make check -j${JOBS}
  cd kafka
  sudo docker-compose down --remove-orphans
  cd "${OLDPWD}"
}

function _main() {
  local CMD=${1}
  shift
  case ${CMD} in
  "--build")
  _docker_build
  ;;
  "--clean")
  _docker_clean
  ;;
  "--run")
  _docker_build
  _docker_run ${@:-}
  ;;
  "--push")
  _docker_build
  _docker_push
  ;;
  "--test")
  # additional command, run in docker container
  _docker_test
  ;;
  *)
  _error
  ;;
  esac
}

_main ${@:-}

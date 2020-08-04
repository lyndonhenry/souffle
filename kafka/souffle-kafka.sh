#!/bin/bash

set -ue

#
# == Utilities ==
#

#
# Sequential for loop.
#
function _for_each() {
  local f="${1}"
  local xs="${@:2}"
  local x
  for x in ${xs}
  do
    (${f}${x} && :)
  done
}

#
# Parallel for loop.
#
function _for_each_async() {
  local f="${1}"
  local xs="${@:2}"
  local x
  for x in ${xs}
  do
    (${f}${x} && :) &
  done
}

#
# Get number of jobs for make -j.
# This is the number of CPUs or 2.
#
function _get_make_jobs() {
  nproc || sysctl -n hw.ncpu || echo 2
}

#
# == Build Command ==
#

#
# Ensures that the user has sudo permissions.
#
function _ensure_we_have_sudo_permissions() {
  sudo echo
}

#
# Ensures that the Ubuntu system is up to date.
#
function _ensure_ubuntu_is_up_to_date() {
  sudo apt-get update -y
  sudo apt-get upgrade -y
  sudo apt-get dist-upgrade -y
}

#
# Ensures that all Docker related dependencies are installed.
#
function _ensure_docker_dependencies_are_installed() {
  sudo apt-get install -y \
    docker-compose
}

#
# Ensures that all Souffle related dependencies are installed.
#
function _ensure_souffle_dependencies_are_installed() {
   sudo apt-get install -y \
    autoconf \
    automake \
    bison \
    build-essential \
    default-jre \
    doxygen \
    flex \
    g++ \
    git \
    graphviz \
    libffi-dev \
    libsqlite3-dev \
    libtool \
    lsb-release \
    libncurses5-dev \
    make \
    mcpp \
    python \
    sqlite \
    zlib1g-dev
}

#
# Ensures that Apache Kafka is installed.
# The installation location is ~/.kafka.
#
function _ensure_apache_kafka_is_installed() {
  local KAFKA_PATH="/home/${USER}/.kafka"
  if [ ! -e "${KAFKA_PATH}" ]
  then
    sudo apt-get install -y curl
    curl -O "http://mirror.vorboss.net/apache/kafka/2.3.1/kafka_2.12-2.3.1.tgz"
    tar -xzf "kafka_2.12-2.3.1.tgz"
    mv "kafka_2.12-2.3.1" "${KAFKA_PATH}"
  fi
}

#
# Ensures that the RdKafka C++ library is installed.
#
function _ensure_librdkafka_is_installed() {
  if [ ! -e "/usr/local/lib/librdkafka.a" ]
  then
    git clone https://github.com/edenhill/librdkafka
    cd librdkafka
    ./configure --install-deps
    make -j"$(_get_make_jobs)"
    sudo make install
    sudo ldconfig
    cd ..
    rm -rf librdkafka
  fi
}

#
# Ensures that Kafkacat is installed.
#
function _ensure_kafkacat_is_installed() {
  sudo apt-get install -y \
    kafkacat
}

#
# Ensures that all Kafka related dependencies are installed.
#
function _ensure_kafka_dependencies_are_installed() {
  _ensure_apache_kafka_is_installed
  _ensure_librdkafka_is_installed
  _ensure_kafkacat_is_installed
}

#
# Ensures that the Ubuntu package cache is clear.
#
function _ensure_ubuntu_package_cache_is_clear() {
  sudo apt-get autoremove -y
  sudo apt-get autoclean -y
  sudo apt-get clean -y
}

#
# Ensures that Souffle is built for use with Kafka.
#
function _ensure_souffle_is_built_for_kafka() {
  make clean || :
  ./bootstrap
  ./configure --enable-kafka --enable-debug --disable-libz --disable-sqlite
  make -j"$(_get_make_jobs)"
}

#
# The build command itself.
# This installs the Souffle on Kafka system.
#
function _build() {
  _ensure_we_have_sudo_permissions
  _ensure_ubuntu_is_up_to_date
  _ensure_docker_dependencies_are_installed
  _ensure_souffle_dependencies_are_installed
  _ensure_kafka_dependencies_are_installed
  _ensure_ubuntu_package_cache_is_clear
  _ensure_souffle_is_built_for_kafka
}

#
# == Test Command ==
#

#
# The test function itself
# Runs the Souffle testsuite with Souffle on Kafka features.
# Each valid combination of interpreter/compiler, engine, and strategy is tested.
#
function _test() {
  local OLDPWD="${PWD}"
  local SOUFFLE_CATEGORY="FastEvaluation"
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
  local JOBS=$(_get_make_jobs)
  TESTSUITEFLAGS="-j${JOBS}" make check -j${JOBS}
  cd kafka
  sudo docker-compose down --remove-orphans
  cd "${OLDPWD}"
}

#
# == Run Commands ==
#

#
# Delete all Kafka topics using the compiled program executable.
#
function _exe_delete_topics() {
  local KAFKA_HOST="${1}"
  local ID="${2}"
  local EXE="${3}"
  local VERBOSE="${4:-}"
  local ARGS=""
  if [ "${VERBOSE}" == "--verbose" ]
  then
  echo "Deleting topics if they exist..."
  ARGS+=" -Xcustom.disable-stdout=false "
  ARGS+=" -Xcustom.disable-stderr=false "
  fi
  ARGS+=" -Xmetadata.broker.list=${KAFKA_HOST} "
  ARGS+=" -Xcustom.create-topics=false "
  ARGS+=" -Xcustom.run-program=false "
  ARGS+=" -Xcustom.delete-topics=true "
  ARGS+=" -Xcustom.unique-id=${ID} "
  ${EXE} ${ARGS}
}

#
# Create all Kafka topics using the compiled program executable.
#
function _exe_create_topics() {
  local KAFKA_HOST="${1}"
  local ID="${2}"
  local EXE="${3}"
  local VERBOSE="${4:-}"
  local ARGS=""
  if [ "${VERBOSE}" == "--verbose" ]
  then
  echo "Creating topics..."
  ARGS+=" -Xcustom.disable-stdout=false "
  ARGS+=" -Xcustom.disable-stderr=false "
  fi
  ARGS+=" -Xmetadata.broker.list=${KAFKA_HOST} "
  ARGS+=" -Xcustom.create-topics=true "
  ARGS+=" -Xcustom.run-program=false "
  ARGS+=" -Xcustom.delete-topics=false "
  ARGS+=" -Xcustom.unique-id=${ID} "
  ${EXE} ${ARGS}
}

#
# Run each strata as a process in parallel using the compiled program executable.
#
function _exe_run_many_programs() {
  local KAFKA_HOST="${1}"
  local ID="${2}"
  local EXE="${3}"
  local STRATUM_NAMES="${4}"
  local VERBOSE="${5:-}"
  local ARGS=""
  if [ "${VERBOSE}" == "--verbose" ]
  then
  echo "Running program..."
  ARGS+=" -Xcustom.disable-stdout=false "
  ARGS+=" -Xcustom.disable-stderr=false "
  fi
  local ARGS=""
  ARGS+=" -Xmetadata.broker.list=${KAFKA_HOST} "
  ARGS+=" -Xcustom.create-topics=false "
  ARGS+=" -Xcustom.run-program=true "
  ARGS+=" -Xcustom.delete-topics=false "
  ARGS+=" -Xcustom.unique-id=${ID} "
  _for_each_async "${EXE} ${ARGS} -i" ${STRATUM_NAMES}
  wait
}

#
# Run a single stratum as a process using the compiled program executable.
#
function _exe_run_one_program() {
  local KAFKA_HOST="${1}"
  local ID="${2}"
  local EXE="${3}"
  local STRATUM_NAME="${4}"
  local VERBOSE="${5:-}"
  local ARGS=""
  if [ "${VERBOSE}" == "--verbose" ]
  then
  echo "Running program..."
  ARGS+=" -Xcustom.disable-stdout=false "
  ARGS+=" -Xcustom.disable-stderr=false "
  fi
  local ARGS=""
  ARGS+=" -Xmetadata.broker.list=${KAFKA_HOST} "
  ARGS+=" -Xcustom.create-topics=false "
  ARGS+=" -Xcustom.run-program=true "
  ARGS+=" -Xcustom.delete-topics=false "
  ARGS+=" -Xcustom.unique-id=${ID} "
  # note that we prevent exit on program failure here
  ${EXE} ${ARGS} -i${STRATUM_NAME} || :
}

#
# Create a Kafka topic using the kafka-topics.sh script.
#
function _kafka_create_topic() {
  local KAFKA_HOST="${1}"
  local TOPIC="${2}"
  kafka-topics.sh --create --bootstrap-server "${KAFKA_HOST}" --replication-factor 1 --partitions 1 --topic "${TOPIC}"
}

#
# Wait for a Kafka topic to exist using the kafka-topics.sh script.
#
function _kafka_wait_for_topic() {
  local KAFKA_HOST="${1}"
  local TOPIC="${2}"
  while :; do
    local SEARCH=$(kafka-topics.sh --list --bootstrap-server "${KAFKA_HOST}" | grep -o "${TOPIC}")
    if [[ "${SEARCH}" == "${TOPIC}" ]]; then
      break;
    fi
    sleep 1
  done
}

#
# Produce a file to a topic using Kafkacat.
#
function _kafka_produce_file() {
  local KAFKA_HOST="${1}"
  local TOPIC="${2}"
  local FILE="${3}"
  # produce line count as one message to topic
  cat "${FILE}" | wc -l | kafkacat -b "${KAFKA_HOST}" -P -t "${TOPIC}"
  # produce lines of file as many messages to topic
  cat "${FILE}" | kafkacat -b "${KAFKA_HOST}" -P -t "${TOPIC}"
}

#
# Produce a message to a topic using Kafkacat.
#
function _kafka_produce_message() {
  local KAFKA_HOST="${1}"
  local TOPIC="${2}"
  local MESSAGE="${3}"
  # produce message to topic
  echo "${MESSAGE}" | kafkacat -b "${KAFKA_HOST}" -P -t "${TOPIC}"
}

#
# Consume a file from a topic using Kafkacat.
#
function _kafka_consume_file() {
  local KAFKA_HOST="${1}"
  local TOPIC="${2}"
  local FILE="${3}"
  # consume line count as one message from topic
  local SIZE="$(kafkacat -b "${KAFKA_HOST}" -c 1 -X topic.auto.offset.reset=earliest "${TOPIC}")"
  # consume lines of file as many messages from topic
  kafkacat -b "${KAFKA_HOST}" -c "${SIZE}" -X topic.auto.offset.reset=earliest "${TOPIC}" > "${FILE}"
}

#
# Consume a message from a topic using Kafkacat.
#
function _kafka_consume_message() {
  local KAFKA_HOST="${1}"
  local TOPIC="${2}"
  # consume message from topic
  local MESSAGE="$(kafkacat -b "${KAFKA_HOST}" -c 1 -X topic.auto.offset.reset=earliest "${TOPIC}")"
  echo "${MESSAGE}"
}

#
# Extend the docker-compose.yml file to include the given stratum as a Docker container.
#
function _docker_compose_extend_for_one() {
  local DOCKER_USER="${1}"
  local IMAGE_NAME="${2}"
  local ID="${3}"
  local DIR="${4}"
  local STRATUM_NAME="${5}"
cat >> "${DIR}/docker-compose.yml" << EOF
  souffle_${ID}_${STRATUM_NAME}:
    image: '${DOCKER_USER}/${IMAGE_NAME}:latest'
    environment:
      KAFKA_HOST: kafka:29092
    networks:
      - app-tier
    command: ./kafka/souffle-kafka.sh --run-as-client ${ID} ${STRATUM_NAME}
EOF
}

#
# Extend the docker-compose.yml file to include all strata as Docker containers.
#
function _docker_compose_extend_for_all() {
  local DOCKER_USER="${1}"
  local IMAGE_NAME="${2}"
  local ID="${3}"
  local DIR="${4}"
  local STRATUM_NAMES="${5}"
  mkdir -p "${DIR}"
  cat ./kafka/docker-compose.yml > "${DIR}/docker-compose.yml"
  _for_each "_docker_compose_extend_for_one ${DOCKER_USER} ${IMAGE_NAME} ${ID} ${DIR} " ${STRATUM_NAMES}
}

#
# Extend the Dockerfile to copy the executable to the image on build.
#
function _docker_file_extend_for_one() {
  local DOCKER_USER="${1}"
  local IMAGE_NAME="${2}"
  local ID="${3}"
  local DIR="${4}"
  local EXE="${5}"
  mkdir -p "${DIR}"
cat >> "${DIR}/Dockerfile" << EOF
FROM ${DOCKER_USER}/${IMAGE_NAME}:latest
COPY ${EXE} /home/${DOCKER_USER}/souffle/${ID}
EOF
}

#
# Create a Kafka topic for logging.
#
function _kafka_create_log_topic() {
  local KAFKA_HOST="${1}"
  local LOG_TOPIC="souffle"
  _kafka_create_topic "${KAFKA_HOST}" "${LOG_TOPIC}"
}

#
# Produce a message to the Kafka log topic.
#
function _kafka_produce_log_message() {
  local KAFKA_HOST="${1}"
  local MESSAGE="${2}"
  local LOG_TOPIC="souffle"
  _kafka_produce_message "${KAFKA_HOST}" "${LOG_TOPIC}" "${MESSAGE}"
}

#
# The run function itself.
# It runs a Souffle on Kafka program on the host.
# It is only ever called by Souffle.
# The user is responsible for managing the Kafka broker.
#
function _run() {
  local EXE="${1}"
  local VERBOSE="${2:-}"
  local KAFKA_HOST="localhost:9092"
  local ID="$(date +%s)_$(basename ${EXE} | sed 's/\./_/g')"
  _exe_delete_topics "${KAFKA_HOST}" "${ID}" "${EXE}" "${VERBOSE}"
  sleep 1s
  _exe_create_topics "${KAFKA_HOST}" "${ID}" "${EXE}" "${VERBOSE}"
  sleep 1s
  local STRATUM_NAMES="$(${EXE} -Xcustom.print-metadata=true)"
  _exe_run_many_programs "${KAFKA_HOST}" "${ID}" "${EXE}" "-2 -3 ${STRATUM_NAMES}" "${VERBOSE}"
}

#
# This is the run-with-broker function.
# It runs a Souffle on Kafka program on the host.
# It is called by Souffle when run with the option -Xrun-with-broker.
# Here, the user is NOT responsible for managing the Kafka broker.
#
function _run_with_broker() {
  local OLDPWD="${PWD}"
  cd "kafka"
  sudo docker-compose down --remove-orphans
  sudo docker-compose up -d --remove-orphans
  cd "${OLDPWD}"
  _run ${@:-}
  cd "kafka"
  sudo docker-compose down --remove-orphans
  cd "${OLDPWD}"
}

#
# This is the run-with-clients function.
# It runs a Souffle on Kafka program both on the host and over Docker containers (clients).
# It is called by Souffle when run with the option -Xrun-with-clients.
#
function _run_with_clients() {
  local OLDPWD="${PWD}"
  local EXE="${1}"
  local VERBOSE="${2:-}"
  local KAFKA_HOST="localhost:9092"
  local DOCKER_USER="default"
  local GITHUB_USER="lyndonmhenry"
  # TODO (lh): change this after development
  local GITHUB_BRANCH="$(git branch --show-current)"
  local NAME="$(basename ${EXE} | sed 's/\./_/g')"
  local ID="$(date +%s)_${NAME}"
  local STRATUM_NAMES="$(${EXE} -Xcustom.print-metadata=true)"
  local DIR=".tmp"
  mkdir -p "${DIR}"
  _docker_file_extend_for_one "${DOCKER_USER}" "souffle" "${ID}" "${DIR}" "${EXE}"
  _docker_build "${DOCKER_USER}" "${GITHUB_USER}" "${GITHUB_BRANCH}" "souffle-copy" ".tmp/Dockerfile"
  cd "${OLDPWD}"
  _docker_compose_extend_for_all "${DOCKER_USER}" "souffle-copy" "${ID}" "${DIR}" "${STRATUM_NAMES}"
  cd "${DIR}"
  sudo docker-compose down --remove-orphans
  sudo docker-compose up -d --remove-orphans
  cd "${OLDPWD}"
  _kafka_create_log_topic "${KAFKA_HOST}"
  _kafka_produce_log_message "${KAFKA_HOST}" "Creating topics at broker..."
  # note that we run verbose here
  _exe_create_topics "${KAFKA_HOST}" "${ID}" "${EXE}" "--verbose"
  sleep 1s
  _kafka_produce_log_message "${KAFKA_HOST}" "Waiting for topics at broker..."
  # create "ready" topic
  _kafka_create_topic "${KAFKA_HOST}" "OK"
  _kafka_wait_for_topic "${KAFKA_HOST}" "OK"
  _kafka_produce_log_message "${KAFKA_HOST}" "Beginning program at broker..."
  _exe_run_many_programs "${KAFKA_HOST}" "${ID}" "${EXE}" "-2 -3" "${VERBOSE}"
  _kafka_produce_log_message "${KAFKA_HOST}" "Ending program at broker..."
  read -p "CONTINUE? "
  cd "${TMP}"
  docker-compose down --remove-orphans
  cd "${OLDPWD}"
}

#
# This is the run-as-client function.
# It runs a single stratum of a Souffle executable.
# It is only ever called by the command in docker-compose.yml.
# This is triggered by a call to the run-with-clients function.
#
function _run_as_client() {
  local ID="${1}"
  local STRATUM_NAME="${2}"
  local VERBOSE="${3:-}"
  local KAFKA_HOST="kafka:29092"
  local EXE="${ID}"
  # wait for "ready" topic
  _kafka_wait_for_topic "${KAFKA_HOST}" "OK"
  _kafka_produce_log_message "${KAFKA_HOST}" "Beginning program at client ${STRATUM_NAME}..."
  _exe_run_one_program "${KAFKA_HOST}" "${ID}" "${EXE}" "${STRATUM_NAME}"
  _kafka_produce_log_message "${KAFKA_HOST}" "Ending program at client ${STRATUM_NAME}..."
}

#
# == Docker Commands ==
#

#
# The clean command itself.
# This deletes all Docker containers and images.
#
function _clean() {
  sudo docker stop $(sudo docker ps -a -q) || :
  sudo docker rm $(sudo docker ps -a -q) || :
  sudo docker rmi $(sudo docker images -q -a) || :
}

#
# Build a Souffle on Kafka Docker image.
#
function _docker_build() {
  local DOCKER_USER="${1}"
  local GITHUB_USER="${2}"
  local GITHUB_BRANCH="${3}"
  local IMAGE_NAME="${4}"
  local DOCKER_FILE="${5}"
  sudo docker build \
  --rm \
  --tag "${DOCKER_USER}/${IMAGE_NAME}" \
  --file "${DOCKER_FILE}" \
  --build-arg DOCKER_USER="${DOCKER_USER}" \
  --build-arg GITHUB_USER="${GITHUB_USER}" \
  --build-arg GITHUB_BRANCH="${GITHUB_BRANCH}" \
  .
}

#
# Run a Souffle on Kafka Docker container.
#
function _docker_run() {
  local DOCKER_USER="${1}"
  local IMAGE_NAME="${2}"
  shift
  shift
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
  "${DOCKER_USER}/${IMAGE_NAME}:latest" \
  "${@:-}"
}

#
# The docker command itself.
# This passes any arguments given to a Souffle on Kafka Docker container.
#
function _docker() {
  local DOCKER_USER="default"
  local GITHUB_USER="lyndonmhenry"
  # TODO (lh): change this after development
  local GITHUB_BRANCH="$(git branch --show-current)"
  _docker_build "${DOCKER_USER}" "${GITHUB_USER}" "${GITHUB_BRANCH}" "souffle" "./kafka/Dockerfile"
  _docker_run "${DOCKER_USER}" "souffle" "${@:-}"
}

#
# = Tutorial Command =
#

function _tutorial_done() {
  #
  # = Introduction =
  #
  # Welcome to the tutorial for Souffle on Kafka.
  #
  # The system is controlled by the script at
  # ./kafka/souffle-kafka.sh. This script must be
  # called from the root directory of the Souffle
  # repository.
  #
  # The script has multiple commands, some of which
  # are called by the user, and some of which are
  # only called under the hood by Souffle.
  #
  # The user commands are --build, --test, --docker,
  # --tutorial, and --help. The user may call the
  # souffle-kafka.sh script with one of these as
  # the first argument.
  #
  # This tutorial will cover each of the user commands.
  #
  # The non-user commands are --run, --run-with-broker,
  # --run-with-clients, and --run-as-client. These are
  # pased by calls to the executable at ./src/souffle.
  #
  # This tutorial will also cover how Souffle triggers
  # each of the non-user commands.
  #
  # Without further ado, we begin the tutorial content.
  #
  # = Build =
  #
  # The first command we will learn is --build.
  #
  # This command installs all necessary packages, and
  # builds software for running datalog programs with
  # the Souffle on Kafka system. It is implemented in
  # souffle-kafka.sh in the function _build.
  #
  ./kafka/souffle-kafka.sh --build
  #
  # If the --build command completes successfully, we
  # are ready to run our first program.
  #
  # = Run =
  #
  # We choose a program from Souffle's testsuite as our
  # example case, and make variables for the fact
  # directory, output directory, and datalog file.
  #
  # For good measure, we make sure that the output directory
  # is exists and is empty.
  #
  local FACT_DIR="tests/evaluation/relop/facts"
  local OUTPUT_DIR="tests/testsuite.dir/evaluation/relop"
  local DATALOG_FILE="tests/evaluation/relop/relop.dl"
  #
  rm -rf "${OUTPUT_DIR}"
  mkdir -p "${OUTPUT_DIR}"
  #
  # Next, we run our program with Souffle to make sure everything
  # is working fine.
  #
  # Note that the -c option means "compile datalog to executable
  # and then run that executable".
  #
  ./src/souffle -F"${FACT_DIR}" -D"${OUTPUT_DIR}" -c "${DATALOG_FILE}"
  #
  # To run a Souffle program on Kafka, we use the kafka engine.
  #
  # Souffle on Kafka uses the concept of 'engines' to define
  # alternative evaluation models for the programs it runs.
  #
  # These are then modified by 'strategies', which we shall
  # come to in a moment.
  #
  # Using the kakfa engine is as simple as passing
  # -Xuse-engine-kafka to Souffle.
  #
  # Note that we also need to pass 'Xrun-with-broker' here,
  # so that Souffle will spawn a new Kafka broker.
  #
  rm -rf "${OUTPUT_DIR}"
  mkdir -p "${OUTPUT_DIR}"
  #
  ./src/souffle -F"${FACT_DIR}" -D"${OUTPUT_DIR}" -c -Xuse-engine-kafka -Xrun-with-broker "${DATALOG_FILE}"
  #
  # TODO (lh): comment (check csv files in output dir, running in parallel)
  #
  read -p "CONTINUE? "
  #
  local FACT_DIR_1="tests/evaluation/relop/facts"
  local OUTPUT_DIR_1="tests/testsuite.dir/evaluation/relop"
  local DATALOG_FILE_1="tests/evaluation/relop/relop.dl"
  #
  rm -rf "${OUTPUT_DIR_1}"
  mkdir -p "${OUTPUT_DIR_1}"
  #
  local FACT_DIR_2="tests/evaluation/binop/facts"
  local OUTPUT_DIR_2="tests/testsuite.dir/evaluation/binop"
  local DATALOG_FILE_2="tests/evaluation/binop/binop.dl"
  #
  rm -rf "${OUTPUT_DIR_2}"
  mkdir -p "${OUTPUT_DIR_2}"
  #
  cd "kafka"
  sudo docker-compose down --remove-orphans
  sudo docker-compose up -d --remove-orphans
  cd ..
  ./src/souffle -F"${FACT_DIR_1}" -D"${OUTPUT_DIR_2}" -c -Xuse-engine-kafka "${DATALOG_FILE_1}" &
  ./src/souffle -F"${FACT_DIR_2}" -D"${OUTPUT_DIR_2}" -c -Xuse-engine-kafka "${DATALOG_FILE_2}"
  wait
  cd "kafka"
  sudo docker-compose down --remove-orphans
  cd ..
  #
  # TODO (lh): comment (check csv files in output dir)
  #
  rm -rf "${OUTPUT_DIR}"
  mkdir -p "${OUTPUT_DIR}"
  #
  ./src/souffle -F"${FACT_DIR}" -D"${OUTPUT_DIR}" -c -Xuse-engine-kafka -Xrun-with-broker -Xuse-general -Xuse-general-producers -Xuse-general-consumers "${DATALOG_FILE}"
  #
  # TODO (lh): comment (running souffle in docker, note that ${HOME}/.souffle on host is mounted at ${HOME}/.souffle in container)
  #
  local DOCKER_USER="default"
  #
  # TODO (lh): comment (all are number types)
  #
  local FACT_DIR=".souffle/binop/facts"
  local OUTPUT_DIR=".souffle/binop"
  local DATALOG_FILE=".souffle/binop/binop.dl"
  #
  rm -rf "${HOME}/${OUTPUT_DIR}"
  mkdir -p "${HOME}/${OUTPUT_DIR}"
  cp -r "tests/evaluation/binop/facts" "${HOME}/${FACT_DIR}"
  cp "tests/evaluation/binop/binop.dl" "${HOME}/${DATALOG_FILE}"
  ./kafka/souffle-kafka.sh --docker ./src/souffle -F"/home/${DOCKER_USER}/${FACT_DIR}" -D"/home/${DOCKER_USER}/${OUTPUT_DIR}" -c -Xuse-engine-kafka -Xrun-with-broker -Xuse-general -Xuse-general-producers -Xuse-general-consumers "/home/${DOCKER_USER}/${DATALOG_FILE}"
  #
  read -p "CONTINUE? "
  #
  # TODO (lh): comment (all are string types)
  #
  local FACT_DIR=".souffle/relop/facts"
  local OUTPUT_DIR=".souffle/relop"
  local DATALOG_FILE=".souffle/relop/relop.dl"
  #
  rm -rf "${HOME}/${OUTPUT_DIR}"
  mkdir -p "${HOME}/${OUTPUT_DIR}"
  cp -r "tests/evaluation/relop/facts" "${HOME}/${FACT_DIR}"
  cp "tests/evaluation/relop/relop.dl" "${HOME}/${DATALOG_FILE}"
  ./kafka/souffle-kafka.sh --docker ./src/souffle -F"/home/${DOCKER_USER}/${FACT_DIR}" -D"/home/${DOCKER_USER}/${OUTPUT_DIR}" -c -Xuse-engine-kafka -Xrun-with-broker -Xuse-general -Xuse-general-producers -Xuse-general-consumers "/home/${DOCKER_USER}/${DATALOG_FILE}"
  #
  read -p "CONTINUE? "
}

function _tutorial_todo() {
  #
  local DOCKER_USER="default"
  #
  # TODO (lh): comment (all are string types)
  #
  local FACT_DIR=".souffle/relop/facts"
  local OUTPUT_DIR=".souffle/relop"
  local DATALOG_FILE=".souffle/relop/relop.dl"
  #
  rm -rf "${HOME}/${OUTPUT_DIR}"
  mkdir -p "${HOME}/${OUTPUT_DIR}"
  cp -r "tests/evaluation/relop/facts" "${HOME}/${FACT_DIR}"
  cp "tests/evaluation/relop/relop.dl" "${HOME}/${DATALOG_FILE}"
  ./kafka/souffle-kafka.sh --docker ./src/souffle -F"/home/${DOCKER_USER}/${FACT_DIR}" -D"/home/${DOCKER_USER}/${OUTPUT_DIR}" -c -Xuse-engine-kafka -Xrun-with-clients -Xuse-general -Xuse-general-producers -Xuse-general-consumers "/home/${DOCKER_USER}/${DATALOG_FILE}"
  #
  # TODO (lh): comment (running souffle testsuite)
  #
  read -p "CONTINUE? "
  #
  # TODO (lh): comment (all are number types)
  #
  local FACT_DIR=".souffle/binop/facts"
  local OUTPUT_DIR=".souffle/binop"
  local DATALOG_FILE=".souffle/binop/binop.dl"
  #
  rm -rf "${HOME}/${OUTPUT_DIR}"
  mkdir -p "${HOME}/${OUTPUT_DIR}"
  cp -r "tests/evaluation/binop/facts" "${HOME}/${FACT_DIR}"
  cp "tests/evaluation/binop/binop.dl" "${HOME}/${DATALOG_FILE}"
  ./kafka/souffle-kafka.sh --docker ./src/souffle -F"/home/${DOCKER_USER}/${FACT_DIR}" -D"/home/${DOCKER_USER}/${OUTPUT_DIR}" -c -Xuse-engine-kafka -Xrun-with-clients -Xuse-general -Xuse-general-producers -Xuse-general-consumers "/home/${DOCKER_USER}/${DATALOG_FILE}"
  #
  read -p "CONTINUE? "
  #
  ./kafka/souffle-kafka.sh --test
  #
  # TODO (lh): comment (this is the end of the tutorial)
  #
  read -p "EXIT? "
}

# TODO (lh): merge tutorial-done and tutorial-todo functions here
#
# The tutorial function itself.
# This calls the tutorial-done and tutorial-todo functions.
# While debugging, comment out the tutorial-done call.
# When debugging is complete, merge the done and todo functions.
#
function _tutorial() {
  #_tutorial_done;
  _tutorial_todo;
}

#
# This function prints the help text.
#
function _help() {
cat << EOF

  Souffle on Kafka

  Usage:

  ./kafka/souffle-kafka.sh [--build|--clean|--docker|--test|--tutorial|--help]

  --build     Build Souffle on Kafka.
  --clean     Delete all Docker containers and images.
  --docker    Pass command to Souffle on Kafka docker instance.
  --test      Run the Souffle on Kafka testsuite.
  --tutorial  Run the tutorial.
  --help      Display this help text.

  Please see the tutorial (option --tutorial) for more information.

EOF
}

#
# The main function, invokes each of the possible commands.
#
function _main() {
  local SOUFFLE_PATH="${PWD}"
  local CMD="${1}"
  shift
  case "${CMD}" in
  "--build")
  _build "${@:-}"
  ;;
  "--test")
  _test "${@:-}"
  ;;
  "--run")
  _run "${@:-}"
  ;;
  "--run-with-broker")
  _run_with_broker "${@:-}"
  ;;
  "--run-with-clients")
  _run_with_clients "${@:-}"
  ;;
  "--run-as-client")
  _run_as_client "${@:-}"
  ;;
  "--docker")
  _docker "${@:-}"
  ;;
  "--clean")
  _clean "${@:-}"
  ;;
  "--tutorial")
  _tutorial "${@:-}"
  ;;
  *)
  _help
  ;;
  esac
}

_main "${@:-}"


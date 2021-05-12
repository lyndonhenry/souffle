#!/bin/bash

# @@@TODO (lh): note that the x flag here must be removed if this script is called by souffle itself
set -ouex pipefail

#
# == Global Variables ==
#

KAFKA_LOG_TOPIC="souffle-log"
VERBOSE="" # use VERBOSE="--verbose" for verbose mode
KAFKA_PATH="/home/${USER}/.kafka"

export PATH="${KAFKA_PATH}/bin:${PATH}"

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
# Simple error and exit.
#
function _error() {
  echo "Error!"
  exit 1
}

#
# Get number of jobs for make -j.
# This is the number of CPUs or 2.
#
function _get_make_jobs() {
  nproc || sysctl -n hw.ncpu || echo 2
}

#
# == Kafka Commands ==
#

#
# Create a Kafka topic using the kafka-topics.sh script.
#
function _kafka_create_topic() {
  local KAFKA_HOST="${1}"
  local TOPIC="${2}"
  kafka-topics.sh --create --bootstrap-server "${KAFKA_HOST}" --replication-factor 1 --partitions 1 --topic "${TOPIC}" &> /dev/null
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
  # consume lines of file as many messages from topic
  kafka-console-consumer.sh --topic "${TOPIC}" --from-beginning --bootstrap-server "${KAFKA_HOST}" --timeout-ms 60000 >> "${FILE}" || :
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
  if [ ! -e "${KAFKA_PATH}" ]
  then
    local KAFKA_FILE="kafka_2.12-2.8.0"
    sudo apt-get install -y curl
    curl -O "https://mirrors.gigenet.com/apache/kafka/2.8.0/${KAFKA_FILE}.tgz"
    tar -xzf "${KAFKA_FILE}.tgz"
    mv "${KAFKA_FILE}" "${KAFKA_PATH}"
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
  ./configure --enable-kafka --disable-libz --disable-sqlite --enable-64bit-domain
  make -j"$(_get_make_jobs)"
}

function _ensure_rust_is_installed() {
  curl https://sh.rustup.rs -sSf | bash -s -- -y
}

function _ensure_souffle_on_kafka_is_built_for_rust() {
  export PATH="/home/${DOCKER_USER}/.cargo/bin:${PATH}"
  cargo build --manifest-path kafka/rust/souffle_on_kafka/Cargo.toml
}

function  _ensure_java_is_installed() {
  sudo apt-get install -y \
    default-jdk \
    maven
}

function _build_logparser() {
  mvn clean install -f kafka/log-parser/pom.xml 
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
  _ensure_rust_is_installed
  _ensure_souffle_on_kafka_is_built_for_rust
  _ensure_java_is_installed
  _build_logparser
}

#
# == Exe Functions ==
#

#
# Delete all Kafka topics using the compiled program executable.
#
function _exe_delete_kafka_topics() {
  local KAFKA_HOST="${1}"
  local ID="${2}"
  local EXE="${3}"
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
  ./${EXE} ${ARGS}
}

#
# Create all Kafka topics using the compiled program executable.
#
function _exe_create_kafka_topics() {
  local KAFKA_HOST="${1}"
  local ID="${2}"
  local EXE="${3}"
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
  ./${EXE} ${ARGS}
}

#
# Run each strata as a process in parallel using the compiled program executable.
#
function _exe_run_all_strata() {
  local KAFKA_HOST="${1}"
  local ID="${2}"
  local EXE="${3}"
  local THREADS="${4}"
  local STRATUM_NAMES="${5}"
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
  _for_each_async "./${EXE} ${ARGS} -j${THREADS} -i" ${STRATUM_NAMES}
  wait
}

#
# Run a single stratum as a process using the compiled program executable.
#
function _exe_run_one_stratum() {
  local KAFKA_HOST="${1}"
  local ID="${2}"
  local EXE="${3}"
  local THREADS="${4}"
  local STRATUM_NAME="${5}"
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
  ./${EXE} ${ARGS} -j${THREADS} -i${STRATUM_NAME}
}

#
# Run the compiled program.
#
function _exe_run_program() {
  local EXE="${1}"
  local THREADS="${2}"
  ${EXE} -j${THREADS}
}

#
# == Kafka Log Topic ==
#

#
# Create a Kafka topic for logging.
#
function _kafka_create_log_topic() {
  local KAFKA_HOST="${1}"
  _kafka_create_topic "${KAFKA_HOST}" "${KAFKA_LOG_TOPIC}_${ID}"
}

#
# Wait for a Kafka topic for logging.
#
function _kafka_wait_for_log_topic() {
  local KAFKA_HOST="${1}"
  _kafka_wait_for_topic "${KAFKA_HOST}" "${KAFKA_LOG_TOPIC}_${ID}"
}

#
# Consume a Kafka topic to a file.
#
function _kafka_consume_log_topic_as_file() {
local KAFKA_HOST="${1}"
local FILE="${2}"
_kafka_consume_file "${KAFKA_HOST}" "${KAFKA_LOG_TOPIC}_${ID}" "${FILE}"
}

#
# Produce a message to the Kafka log topic.
#
MESSAGE_INDEX=0
BEGIN_TIME=0
END_TIME=0
function _kafka_produce_log_message() {
  local KAFKA_HOST="${1}"
  local MESSAGE_TYPE="${2}"
  local MESSAGE
  case ${MESSAGE_TYPE} in
  "beginBashScript")
  MESSAGE="beginBashScript"
  ;;
  "endBashScript")
  MESSAGE="endBashScript"
  ;;
  "beginSouffleProgram")
  MESSAGE="beginSouffleProgram"
  ;;
  "endSouffleProgram")
  # note: this is from the beginning of the souffle program execution, not since the last message
  MESSAGE="endSouffleProgram"
  ;;
  "downloadInput")
  # note: this always assumes './input/E.facts'
  local LINE_COUNT="$(wc "./input/E.facts" | awk '{print $1}')"
  MESSAGE="downloadInput,${LINE_COUNT}"
  ;;
  "uploadOutput")
  # note: this always assumes './output/I.csv'
  local FILE
  for FILE in ./output/*.csv
  do
    local LINE_COUNT="$(cat ${FILE})"
    MESSAGE="uploadOutput,${LINE_COUNT},$(basename ${FILE})"
  done
  ;;
  "printMetadata")
  BEGIN_TIME=$(($(date +%s%N)/1000000))
  MESSAGE="printMetadata,$(echo $(basename ${S3_EXE})_$(basename ${S3_INPUT})_${THREADS}_${ID} | tr '_' ',')"
  ;;
  *)
    echo "ERROR!"
    exit 1
  ;;
  esac
  local STRATUM_NAME=${STRATUM_NAME:-none}
  # note: get the unix time in milliseconds
  END_TIME=$(($(date +%s%N)/1000000))
  TIMESTAMP=$((END_TIME - BEGIN_TIME))
  _kafka_produce_message "${KAFKA_HOST}" "${KAFKA_LOG_TOPIC}_${ID}" "${STRATUM_NAME},${MESSAGE_INDEX},${TIMESTAMP},${MESSAGE}"
  BEGIN_TIME=${END_TIME}
  (( ++MESSAGE_INDEX ))
}

#
# == Run Functions ===
#

function _broker_begin_kafka_log() {
  EXE="./exe"
  INPUT="./input"
  OUTPUT="./output"
  mkdir -p ${INPUT} ${OUTPUT}
  _kafka_create_log_topic "${KAFKA_HOST}"
  _kafka_produce_log_message "${KAFKA_HOST}" "printMetadata"
  _kafka_produce_log_message "${KAFKA_HOST}" "beginBashScript"
}

function _broker_begin_with_s3() {
  EXE="./exe"
  INPUT="./input"
  OUTPUT="./output"
  aws s3 cp "${S3_EXE}" "${EXE}"
  chmod +x "${EXE}"
  aws s3 cp --recursive "${S3_INPUT}" "${INPUT}"
  _kafka_produce_log_message "${KAFKA_HOST}" "downloadInput"
}

function _broker_begin_with_kafka() {
  EXE="./exe"
  INPUT="./input"
  OUTPUT="./output"
  _kafka_wait_for_log_topic "${KAFKA_HOST}"
  _exe_create_kafka_topics "${KAFKA_HOST}" "${ID}" "${EXE}"
  sleep 1s
  _kafka_create_topic "${KAFKA_HOST}" "OK_${ID}"
  _kafka_wait_for_topic "${KAFKA_HOST}" "OK_${ID}"
}

function _broker_end() {
  EXE="./exe"
  INPUT="./input"
  OUTPUT="./output"
  aws s3 cp --recursive "${OUTPUT}" "${S3_OUTPUT}/${ID}"
  _kafka_produce_log_message "${KAFKA_HOST}" "uploadOutput"
  _kafka_produce_log_message "${KAFKA_HOST}" "endBashScript"
  local LOG_FILE="$(basename ${S3_EXE})_$(basename ${S3_INPUT})_${THREADS}_${ID}.log"
  _kafka_consume_log_topic_as_file "${KAFKA_HOST}" "${LOG_FILE}"
  aws s3 cp "${LOG_FILE}" "${S3_OUTPUT}/log/${LOG_FILE}"
  # loop forever so as not to trigger a restart
  while true; do :; done
}

function _broker_end_with_no_s3() {
  EXE="./exe"
  INPUT="./input"
  OUTPUT="./output"
  _kafka_produce_log_message "${KAFKA_HOST}" "endBashScript"
  # note that we do not loop forever here as this is only called in souffle testing
}

function _broker_run_with_no_kafka() {
  local EXE="./exe"
  local INPUT="./input"
  local OUTPUT="./output"
  _kafka_produce_log_message "${KAFKA_HOST}" "beginSouffleProgram"
  _exe_run_program ${EXE} ${THREADS}
  _kafka_produce_log_message "${KAFKA_HOST}" "endSouffleProgram"
}

function _broker_run_with_kafka_one() {
  local EXE="./exe"
  local INPUT="./input"
  local OUTPUT="./output"
  local STRATUM_NAMES="$(./${EXE} -Xcustom.print-metadata=true)"
  _kafka_produce_log_message "${KAFKA_HOST}" "beginSouffleProgram"
  _exe_run_all_strata "${KAFKA_HOST}" "${ID}" "${EXE}" "${THREADS}" "-2 -3 ${STRATUM_NAMES}"
  _kafka_produce_log_message "${KAFKA_HOST}" "endSouffleProgram"
}

function _broker_run_with_kafka_many() {
  local EXE="./exe"
  local INPUT="./input"
  local OUTPUT="./output"
  _kafka_produce_log_message "${KAFKA_HOST}" "beginSouffleProgram"
  _exe_run_all_strata "${KAFKA_HOST}" "${ID}" "${EXE}" "${THREADS}" "-2 -3"
  _kafka_produce_log_message "${KAFKA_HOST}" "endSouffleProgram"
}

function _client_run_with_kafka_many() {
  local EXE="./exe"
  local INPUT="./input"
  local OUTPUT="./output"
  _kafka_wait_for_log_topic "${KAFKA_HOST}"
  _kafka_produce_log_message "${KAFKA_HOST}" "beginBashScript"
  aws s3 cp "${S3_EXE}" "${EXE}"
  chmod +x "${EXE}"
  _kafka_wait_for_topic "${KAFKA_HOST}" "OK_${ID}"
  _kafka_produce_log_message "${KAFKA_HOST}" "beginSouffleProgram"
  _exe_run_one_stratum "${KAFKA_HOST}" "${ID}" "${EXE}" "${THREADS}" "${STRATUM_NAME}"
  _kafka_produce_log_message "${KAFKA_HOST}" "endSouffleProgram"
  _kafka_produce_log_message "${KAFKA_HOST}" "endBashScript"
  while true; do :; done
}

function _make_aws_config_and_credentials() {
  if [ ! "${AWS_ACCESS_KEY_ID}" ]
  then
    exit 1
  fi
  if [ ! "${AWS_SECRET_ACCESS_KEY}" ]
  then
    exit 1
  fi
  if [ ! -e ${HOME}/.aws ]
  then
  mkdir -p ${HOME}/.aws
cat > ${HOME}/.aws/config << EOF
[default]
region = us-west-2
EOF
cat > ${HOME}/.aws/credentials << EOF
[default]
aws_access_key_id = ${AWS_ACCESS_KEY_ID}
aws_secret_access_key = ${AWS_SECRET_ACCESS_KEY}
EOF
  fi
}

function _run_with_no_kafka() {
  _broker_begin_kafka_log
  _broker_begin_with_s3
  _broker_run_with_no_kafka
  _broker_end
}

function _run_with_one_kafka() {
  _broker_begin_kafka_log
  _broker_begin_with_s3
  _broker_begin_with_kafka
  _broker_run_with_kafka_one
  _broker_end
}

function _run_with_many_kafka_as_broker() {
  _broker_begin_kafka_log
  _broker_begin_with_s3
  _broker_begin_with_kafka
  _broker_run_with_kafka_many
  _broker_end
}

function _run_with_many_kafka_as_client() {
  _client_run_with_kafka_many
}

function _run_as_called_by_souffle() {
  local EXE_LOCAL="${1}"
  local EXE="./exe"
  ID="$(date +%s)"
  KAFKA_HOST="localhost:9092"
  cp "${EXE_LOCAL}" "${EXE}"
  local THREADS=1
  _broker_begin_kafka_log
  _broker_begin_with_kafka
  local STRATUM_NAMES="$(./${EXE} -Xcustom.print-metadata=true)"
  _exe_run_all_strata "${KAFKA_HOST}" "${ID}" "${EXE}" "${THREADS}" "-2 -3 ${STRATUM_NAMES}"
  _broker_end_with_no_s3
}

function _run() {
  _make_aws_config_and_credentials
  case ${MODE} in
  "no-kafka")
  _run_with_no_kafka
  ;;
  "one-kafka")
  _run_with_one_kafka
  ;;
  "many-kafka-as-broker")
  _run_with_many_kafka_as_broker
  ;;
  "many-kafka-as-client")
  _run_with_many_kafka_as_client
  ;;
  "by-souffle")
  _run_as_called_by_souffle ${@:-}
  ;;
  *)
  _error
  ;;
esac
}

function _main() {
  case ${1} in
  "--build")
  _build
  ;;
  "--run")
  shift
  _run ${@:-}
  ;;
  *)
    _error
    ;;
  esac
}

_main ${@:-}

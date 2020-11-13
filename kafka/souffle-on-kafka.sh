#!/bin/bash

set -ouex pipefail

##
## = Souffle On Kafka =
##
## Usage:
##
##  ./kafka/souffle-on-kafka.sh
##    --benchmark [LR|RR|NR|SG|RSG|TC|SCC|MN] - The name of the benchmark.
##    --type [number|symbol] - The data type.
##    --split [none|<number>] - The breadth of the split, none for no split.
##    --join [none|left|balanced|complete|lattice] - The type of the join, none for no join.
##    --mode [no-kafka|one-kafka|many-kafka] - The engine mode to use.
##    --algorithm [SNE|GSNE|GPSNE|GPCSNE] - Evaluation algorithm to be used.
##    --data <name> - The name of the dataset to be used.
##    --threads <number> - The number of threads to use.
##
##

#
# = Global Variables =
#

source ./kafka/.env

#
# = Utility Functions =
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

function _help() {
  cat $0 | grep "^##*"
}

function _error() {
  _help
  exit 1
}

#
# = Phase Two =
#

function _build_souffle_args() {
  local MODE="${1}"
  local ALGORITHM="${2}"
  local ARGS=""
  case ${MODE} in
  "no-kafka")
  :
  ;;
  "one-kafka")
  ARGS+=" -Xuse-engine-kafka "
  ;;
  "many-kafka")
  ARGS+=" -Xuse-engine-kafka "
  ;;
  esac
  case ${ALGORITHM} in
  "SNE")
  :
  ;;
  "GSNE")
  ARGS+=" -Xuse-general "
  ;;
  "GPSNE")
  ARGS+=" -Xuse-general -Xuse-general-producers "
  ;;
  "GPCSNE")
  ARGS+=" -Xuse-general -Xuse-general-producers -Xuse-general-consumers"
  ;;
  esac
  echo "${ARGS}"
}

#
# = Phase Three =
#

function _extend_docker_compose() {
  local ID="${1}"
  local FILE="${2}"
  local KAFKA_HOST="${3}"
  local MODE="${4}"
  local S3_EXE="${5}"
  local S3_INPUT="${6}"
  local S3_OUTPUT="${7}"
  local THREADS="${8}"
  local STRATUM_NAME="${9}"
cat >> "${FILE}" << EOF
  souffle-${ID}-${STRATUM_NAME}:
    image: '${IMAGE_NAME}:latest'
    environment:
      ID: ${ID}
      KAFKA_HOST: ${KAFKA_HOST}
      MODE: ${MODE}
      S3_EXE: ${S3_EXE}
      S3_INPUT: ${S3_INPUT}
      S3_OUTPUT: ${S3_OUTPUT}
      STRATUM_NAME: ${STRATUM_NAME}
      THREADS: ${THREADS}
      AWS_ACCESS_KEY_ID: \${AWS_ACCESS_KEY_ID}
      AWS_SECRET_ACCESS_KEY: \${AWS_SECRET_ACCESS_KEY}
    networks:
      - app-tier
    command: ./kafka/docker.sh --run
    deploy:
      resources:
        limits:
          cpus: '${THREADS}'
EOF
}

function _extend_k8s() {
  local ID="${1}"
  local FILE="${2}"
  local KAFKA_HOST="${3}"
  local MODE="${4}"
  local S3_EXE="${5}"
  local S3_INPUT="${6}"
  local S3_OUTPUT="${7}"
  local THREADS="${8}"
  local STRATUM_NAME="${9}"

# apiVersion: v1
# kind: Service
# metadata:
#   name: souffle-${ID}-${STRATUM_NAME}
#   labels:
#     app: souffle-${ID}-${STRATUM_NAME}
#     tier: backend
# spec:
#   selector:
#     app: souffle-${ID}-${STRATUM_NAME}
# ---

cat >> "${FILE}" << EOF
apiVersion: apps/v1
kind: Deployment
metadata:
  name: souffle-${ID}-${STRATUM_NAME}
spec:
  selector:
    matchLabels:
      app: souffle-${ID}-${STRATUM_NAME}
  replicas: 1
  template:
    metadata:
      labels:
        app: souffle-${ID}-${STRATUM_NAME}
    spec:
      containers:
      - name: souffle-${ID}-${STRATUM_NAME}
        image: ${IMAGE_NAME}:latest
        command: ['./kafka/docker.sh']
        args: ['--run']
        env:
          - name: ALLOW_ANONYMOUS_LOGIN
            value: 'yes'
          - name: ID: 
            value: ${ID}
          - name: KAFKA_HOST
            value: ${KAFKA_HOST}
          - name: MODE
            value: ${MODE}
          - name: S3_EXE 
            value: ${S3_EXE}
          - name: S3_INPUT 
            value: ${S3_INPUT}
          - name: S3_OUTPUT 
            value: ${S3_OUTPUT}
          - name: STRATUM_NAME   
            value: ${STRATUM_NAME}
          - name: THREADS
            value: ${THREADS}
          - name: AWS_ACCESS_KEY_ID 
            value: \${AWS_ACCESS_KEY_ID}
          - name: AWS_SECRET_ACCESS_KEY 
            value: \${AWS_SECRET_ACCESS_KEY}
        resources:
          requests:
            cpu: '${THREADS}'
          limits:
            cpu: '${THREADS}'
---
EOF
}

function _generate_with_no_kafka() {
  local ID="${1}"
  local FILE="${2}"
  local KAFKA_HOST="${3}"
  local MODE="no-kafka"
  local S3_EXE="${4}"
  local S3_INPUT="${5}"
  local S3_OUTPUT="${6}"
  local THREADS="${7}"
  local DESCRIPTOR_GENERATOR="${8}"
  local TEMPLATE="${9}"

  mkdir -p "$(dirname "${FILE}")"
  cat ${TEMPLATE} > "${FILE}"
  ${DESCRIPTOR_GENERATOR} ${ID} ${FILE} ${KAFKA_HOST} ${MODE} ${S3_EXE} ${S3_INPUT} ${S3_OUTPUT} ${THREADS} "none"
}

function _generate_with_one_kafka() {
  local ID="${1}"
  local FILE="${2}"
  local KAFKA_HOST="${3}"
  local MODE="one-kafka"
  local S3_EXE="${4}"
  local S3_INPUT="${5}"
  local S3_OUTPUT="${6}"
  local THREADS="${7}"
  local DESCRIPTOR_GENERATOR="${8}"
  local TEMPLATE="${9}"

  mkdir -p "$(dirname "${FILE}")"
  cat ${TEMPLATE} > "${FILE}"
  ${DESCRIPTOR_GENERATOR} ${ID} ${FILE} ${KAFKA_HOST} ${MODE} ${S3_EXE} ${S3_INPUT} ${S3_OUTPUT} ${THREADS} "none"
}

function _generate_with_many_kafka() {
  local ID="${1}"
  local FILE="${2}"
  local KAFKA_HOST="${3}"
  local MODE="many-kafka"
  local S3_EXE="${4}"
  local S3_INPUT="${5}"
  local S3_OUTPUT="${6}"
  local THREADS="${7}"
  local EXE="exe"
  local DESCRIPTOR_GENERATOR="${8}"
  local TEMPLATE="${9}"
  rm -f ${EXE}
  aws s3 cp ${S3_EXE} ${EXE}
  chmod +x ${EXE} || :
  mkdir -p "$(dirname "${FILE}")"
  cat $TEMPLATE > "${FILE}"
  local STRATUM_NAMES="$(./${EXE} -Xcustom.print-metadata=true)"
  ${DESCRIPTOR_GENERATOR} ${ID} ${FILE} ${KAFKA_HOST} "${MODE}-as-broker" ${S3_EXE} ${S3_INPUT} ${S3_OUTPUT} ${THREADS} "none"
  _for_each "${DESCRIPTOR_GENERATOR} ${ID} ${FILE} ${KAFKA_HOST} "${MODE}-as-client" ${S3_EXE} ${S3_INPUT} ${S3_OUTPUT} ${THREADS} " ${STRATUM_NAMES}
}

function _generate_descriptor() {
  local ID="$(date +%s)"
  local FILE="${1}"
  local KAFKA_HOST="${2}"
  local MODE="${3}"
  local S3_EXE="${4}"
  local S3_INPUT="${5}"
  local S3_OUTPUT="${6}"
  local THREADS="${7}"
  local DESCRIPTOR_GENERATOR="${8}"
  local TEMPLATE="${9}"

  local CMD="_generate_with"
  case ${MODE} in
  "no-kafka")
  CMD="_generate_with_no_kafka"
  ;;
  "one-kafka")
  CMD="_generate_with_one_kafka"
  ;;
  "many-kafka")
  CMD="_generate_with_many_kafka"
  ;;
  *)
  _error
  ;;
esac
  ${CMD} ${ID} ${FILE} ${KAFKA_HOST} ${S3_EXE} ${S3_INPUT} ${S3_OUTPUT} ${THREADS} ${DESCRIPTOR_GENERATOR} ${TEMPLATE}
}

#
# = Build Phases =
#

function _phase_one() {
  local BENCHMARK="${1}"
  local TYPE="${2}"
  local SPLIT="${3}"
  local JOIN="${4}"
  #
  local FILE="${ROOT}/datalog/${BENCHMARK}_${TYPE}_${SPLIT}_${JOIN}.dl"
  if [ ! -e "${FILE}" ]
  then
    cargo build --manifest-path kafka/rust/souffle_on_kafka/Cargo.toml
    kafka/rust/souffle_on_kafka/target/debug/souffle_on_kafka --benchmark ${BENCHMARK} ${TYPE} ${SPLIT} ${JOIN}
  fi
  #
  echo ${FILE}
}

function _phase_two() {
  local FILE="${1}"
  local MODE="${2}"
  local ALGORITHM="${3}"
  local THREADS="${4}"
  local EXE="${ROOT}/exe/$(basename "${FILE}" | sed 's/\.dl//')_${MODE}_${ALGORITHM}_${THREADS}"
  local ARGS="$(_build_souffle_args ${MODE} ${ALGORITHM})"
  mkdir -p "input" "output"
  if [ ! -e "${EXE}" ]
  then
  mkdir -p "${ROOT}/exe"
  ./src/souffle -F"input" -D"output" ${ARGS} -j${THREADS} -o${EXE} ${FILE}
  aws s3 cp "${EXE}" "s3://souffle-on-kafka/exe/$(basename ${EXE})" &> /dev/null
  fi
  #
  echo "${EXE}"
}

function _phase_three() {
  local EXE="${1}"
  local MODE="${2}"
  local DATA="${3}"
  local THREADS="${4}"
  local SUBDIR="${5}"
  local LOCATION="docker-compose/${SUBDIR}/$(basename ${EXE})_${DATA}/docker-compose.yml"
  local LOCATION_K8S="k8s/${SUBDIR}/$(basename ${EXE})_${DATA}/k8s-deployment.yml"
  local FILE="${ROOT}/${LOCATION}"
  local FILE_K8S="${ROOT}/${LOCATION_K8S}"
  local KAFKA_HOST="kafka:29092"
  local S3_EXE="s3://souffle-on-kafka/exe/$(basename ${EXE})"
  local S3_INPUT="s3://souffle-on-kafka/input/${DATA}"
  local S3_OUTPUT="s3://souffle-on-kafka/output"
  if [ ! -e "${FILE}" ]
  then
    _generate_descriptor "${FILE}" "${KAFKA_HOST}" "${MODE}" "${S3_EXE}" "${S3_INPUT}" "${S3_OUTPUT}" "${THREADS}" "_extend_docker_compose" "./kafka/docker-compose.yml"
    aws s3 cp ${FILE} "s3://souffle-on-kafka/${LOCATION}"
  fi
  echo "s3://souffle-on-kafka/${LOCATION}"

  if [ ! -e "${FILE_K8S}" ]
  then
    _generate_descriptor "${FILE_K8S}" "${KAFKA_HOST}" "${MODE}" "${S3_EXE}" "${S3_INPUT}" "${S3_OUTPUT}" "${THREADS}" "_extend_k8s" "./kafka/k8s-descriptor.yml"
    aws s3 cp ${FILE_K8S} "s3://souffle-on-kafka/${LOCATION_K8S}"
  fi
}

#
# = Main Command =
#

function _main() {
  # phase 1
  local BENCHMARK
  local TYPE
  local SPLIT
  local JOIN
  # phase 2
  local MODE
  local ALGORITHM
  # phase 3
  local DATA
  local THREADS
  local SUBDIR
  #
  while [ ${#} != 0 ]
  do
    case ${1} in
    #
    "--benchmark")
      shift
      BENCHMARK="${1}"
      shift
    ;;
    "--type")
      shift
      TYPE="${1}"
      shift
    ;;
    "--split")
      shift
      SPLIT="${1}"
      shift
    ;;
    "--join")
      shift
      JOIN="${1}"
      shift
    ;;
    #
    "--mode")
      shift
      MODE="${1}"
      shift
    ;;
    "--algorithm")
      shift
      ALGORITHM="${1}"
      shift
    ;;
    #
    "--data")
      shift
      DATA="${1}"
      shift
    ;;
    "--threads")
      shift
      THREADS="${1}"
      shift
    ;;
    "--subdir")
      shift
      SUBDIR="${1}"
      shift
    ;;
    *)
    _error
    ;;
    esac
  done
  #
  local FILE=$(_phase_one ${BENCHMARK} ${TYPE} ${SPLIT} ${JOIN})
  local EXE=$(_phase_two ${FILE} ${MODE} ${ALGORITHM} ${THREADS})
  _phase_three ${EXE} ${MODE} ${DATA} ${THREADS} ${SUBDIR}
}


_main ${@:-}

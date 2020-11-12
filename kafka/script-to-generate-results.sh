#!/bin/bash

set -ouex pipefail

function _generate_results() {
  local INPUT_FILE="${1}"
  local OUTPUT_DIR="${2}"
  local METRICS_TYPE_A=""
  local METRICS_TYPE_B=""
  METRICS_TYPE_A+=" computationTime "
  METRICS_TYPE_A+=" runTime "
  METRICS_TYPE_A+=" totalCommunicationTime "
  METRICS_TYPE_A+=" totalComputationTime "
  METRICS_TYPE_A+=" totalRunTime "
  METRICS_TYPE_A+=" inputSize "
  METRICS_TYPE_A+=" outputSize "
  METRICS_TYPE_A+=" stringBytesProduced "
  METRICS_TYPE_A+=" relationTuplesProduced "
  METRICS_TYPE_A+=" totalStringBytesProduced "
  METRICS_TYPE_A+=" totalRelationTuplesProduced "
  local METRIC
  for METRIC in ${METRICS}
  do
    grep "${METRIC}" "${INPUT_FILE}" > "${OUTPUT_DIR}/${METRIC}.facts"
  done
  read -p "HERE!"
}

function _main() {
  _generate_results "${1}" "${2}"
}

_main "${@:-}"


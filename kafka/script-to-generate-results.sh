#!/bin/bash

set -ouex pipefail

function _generate_results() {
  local INPUT_FILE="${1}"
  local OUTPUT_DIR="${2}"
  local METRIC_TYPE_A=""
  local METRIC_TYPE_B=""
  METRIC_TYPE_B+=" communicationTime "
  METRIC_TYPE_B+=" computationTime "
  METRIC_TYPE_B+=" runTime "
  METRIC_TYPE_A+=" totalCommunicationTime "
  METRIC_TYPE_A+=" totalComputationTime "
  METRIC_TYPE_A+=" totalRunTime "
  METRIC_TYPE_A+=" inputSize "
  METRIC_TYPE_A+=" outputSize "
  METRIC_TYPE_B+=" stringBytesProduced "
  METRIC_TYPE_B+=" relationTermsProduced "
  METRIC_TYPE_A+=" totalStringBytesProduced "
  METRIC_TYPE_A+=" totalRelationTuplesProduced "
  rm -f "${OUTPUT_DIR}/MetricTypeA.facts"
  rm -f "${OUTPUT_DIR}/MetricTypeB.facts"
  rm -f "${OUTPUT_DIR}/ThirdTable.csv"
  local METRIC
  for METRIC in ${METRIC_TYPE_A}
  do
    grep "${METRIC}" "${INPUT_FILE}" >> "${OUTPUT_DIR}/MetricTypeA.facts"
  done
  for METRIC in ${METRIC_TYPE_B}
  do
    grep "${METRIC}" "${INPUT_FILE}" >> "${OUTPUT_DIR}/MetricTypeB.facts"
  done
  souffle ./kafka/generate-results.dl -F"${OUTPUT_DIR}" -D"${OUTPUT_DIR}"
}

function _main() {
  _generate_results "${1}" "${2}"
}

_main "${@:-}"


#!/bin/bash

PROGRAM_NAME=$1
JSON_DATA=$(./program -i-2)
RELATION_NAMES="$(echo ${JSON_DATA} | jq -r '.RelationNames | .[]')"
STRATUM_NAMES="$(echo ${JSON_DATA} | jq -r '.StratumNames | .[]')"
OUTPUT_DIR="output"
INPUT_DIR="facts"
INPUT_FACTS=$(ls facts/*.facts)

source "$(dirname "$0")/kafka_api.sh"

#
#   Read output data from Kafka messages and store them in files. 
#
function read_output_data {
    local TOPIC="$1"

    read_message_async "${TOPIC}" "output" "${OUTPUT_DIR}" 
}

#
#   Extend docker compose with strata
#
function extend_compose {
    RELATION_NAME="$1"

  echo "  
  strata${RELATION_NAME}:
    image: '${DOCKER_IMAGE}:latest'  
    environment: 
      KAFKA_HOST: kafka:29092
    networks:
      - app-tier
    command: ./execute_strata_native.sh program "${RELATION_NAME}"
    " >> docker-compose.yml


} 

#
# Execute a call-back method asynchronously
#
function for_each_async() {
    local f="${1}"
    local xs="${@:2}"
    local x
    for x in ${xs}
    do
        ${f} ${x} &
    done
}

#
# Execute a call-back method 
#
function for_each() {
    local f="${1}"
    local xs="${@:2}"
    local x
    for x in ${xs}
    do
        ${f} ${x}
    done
}


#
#   Extend docker compose with images containers that represent stratas
#
extend_compose "-1"
for_each extend_compose ${RELATION_NAMES}

#
#   Start-up the whole environment - Kafka + nodes for Stratas
#
docker-compose up -d

#
#   Make sure Kafka is ready
#
wait_kafka_ready

# ensure that all program specific topics exist
# Ask Lyndon if we need this
for_each_async create_topic ${RELATION_NAMES}
wait  

# START=$(date +%s.%N)


# echo "Collecting results from stratas"
# #
# #   Waiting for all results to be ready
# #
# iterate_stratas iterate_output_relations_strata read_output_data
# wait    #   Wait until all results are ready

# END=$(date +%s.%N)

# echo "All results collected in ${OUTPUT_DIR}"

# # docker-compose down

# echo "Output dir content:"
# ls -lh ${OUTPUT_DIR}

# DIFF=$(echo "$END - $START" | bc)
# echo "Total time: $DIFF (Sec.nanos)"

/bin/bash

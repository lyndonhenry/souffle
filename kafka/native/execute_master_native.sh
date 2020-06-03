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
    STRATUM_NAME="$1"

  echo "  
  strata${STRATUM_NAME}:
    image: '${DOCKER_IMAGE}:latest'  
    environment: 
      KAFKA_HOST: kafka:29092
    networks:
      - app-tier
    command: ./execute_strata_native.sh program "${STRATUM_NAME}"
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
for_each extend_compose "${STRATUM_NAMES}"

#
#   Start-up the whole environment - Kafka + nodes for Stratas
#
docker-compose up -d

#
#   Make sure Kafka is ready
#
wait_kafka_ready

# TODO:: Ask lyndon how to catch correctly start of execution
## very coarse solution would be just to add some long sleep here to make sure all containers are run
START=$(date +%s.%N)

# Strata with index "-1" is a master who collects data from the fact dir and sends them in Kafka
# TODO:: Ask Lyndon in it is parallel inside (if output files are sent in parallel to respective topics)
./execute_strata_native.sh program "-1" 

END=$(date +%s.%N)

docker-compose down

# echo "All results collected in ${OUTPUT_DIR}"
# TODO - how to get all results?
echo "Output dir content:"
wc -l ${OUTPUT_DIR}/*.csv

DIFF=$(echo "$END - $START" | bc)
echo "Total time: $DIFF (Sec.nanos)"

/bin/bash

#!/bin/bash

PROGRAM_NAME=$1
JSON_DATA=$(echo $(cat ${PROGRAM_NAME}.json))
OUTPUT_DIR="output"
INPUT_DIR="facts"
INPUT_FACTS=$(ls facts/*.facts)

source "$(dirname "$0")/kafka_api.sh"


#
#   Iterate all input files
#
function iterate_input_files {
    local command=$1

    for f in $INPUT_DIR/*.facts; do
        local TOPIC=$(basename $f | cut -f 1 -d '.')

        $command "IIII${TOPIC}" "$f" 
    done
}

#
#   Read input file and send it to kafka
#
function send_input_file {
    local TOPIC="$1"
    local FILE="$2"

    send_message_async "${TOPIC}" "${FILE}"
}

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
    STRATUM_INDEX="$1"

  echo "  
  strata${STRATUM_INDEX}:
    image: '${DOCKER_IMAGE}:latest'  
    environment: 
      STRATUM_INDEX: ${STRATUM_INDEX}  
      KAFKA_HOST: kafka:29092
    networks:
      - app-tier
    command: ./execute_strata.sh program 
    " >> docker-compose.yml


} 


#
#   Start-up docker for every strata
#
# iterate_stratas execute_strata_docker    

#
#   Extend docker compose with images containers that represent stratas
#
extend_compose -1
iterate_stratas extend_compose

#
#   Start-up the whole environment - Kafka + nodes for Stratas
#
docker-compose up -d

#
#   Make sure Kafka is ready
#
wait_kafka_ready


START=$(date +%s.%N)

echo "Distributing input facts to stratas"
#
#   Iterate over input facts and send them into Kafka 
#   
iterate_input_files send_input_file

echo "Collecting results from stratas"
#
#   Waiting for all results to be ready
#
iterate_stratas iterate_output_relations_strata read_output_data
wait    #   Wait until all results are ready

END=$(date +%s.%N)

echo "All results collected in ${OUTPUT_DIR}"

# docker-compose down

echo "Output dir content:"
ls -lh ${OUTPUT_DIR}

DIFF=$(echo "$END - $START" | bc)
echo "Total time: $DIFF (Sec.nanos)"

/bin/bash

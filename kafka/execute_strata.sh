#!/bin/bash

#
# This script execute one strata
#
# This strata waits until data are ready in Kafka topics.
# Once the data are ready, it reads them, stores them in output directory and 
# starts processing. When the execution ends, results are stored in Kafka topics
#

PROGRAM_NAME=$1

JSON_DATA=$(echo $(cat ${PROGRAM_NAME}.json))

source "$(dirname "$0")/kafka_api.sh"

#
#   Every strata should create topics which this strata provides
#
echo "Creating topics for outcomming messages."
iterate_output_relations_strata create_topic $STRATUM_INDEX

#
#   Wait for topics for incommings messages to be created
#
echo "Waiting untill all incomming topics are created. "
iterate_input_relations_strata wait_topic_exists $STRATUM_INDEX


echo "Invoking strata index ${STRATUM_INDEX}"

# Execute program - (1) wait for incomming messages, (2) run the program, (3) send outcomming messages
iterate_input_relations_strata read_message $STRATUM_INDEX
./${PROGRAM_NAME} -i${STRATUM_INDEX}
iterate_output_relations_strata send_message $STRATUM_INDEX
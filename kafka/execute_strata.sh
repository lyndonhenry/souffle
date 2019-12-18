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
OUTPUT_DIR="output"
INPUT_DIR="facts"

echo "Beginning of strata ${STRATUM_INDEX}"

source "$(dirname "$0")/kafka_api.sh"

#
#   Make sure Kafka is ready
#
wait_kafka_ready

#
#   Every strata should create topics which this strata provides
#
echo "Creating topics for outcomming messages."
iterate_outgoing_relations_strata create_topic_async $STRATUM_INDEX

#
#   Wait for topics for incommings messages to be created
#
echo "Waiting untill all incomming topics are created. "
iterate_incoming_relations_strata wait_topic_exists $STRATUM_INDEX


echo "Invoking computation for strata index ${STRATUM_INDEX}"

# Execute program - (1) wait for incomming messages, (2) run the program, (3) send outcomming messages
iterate_incoming_relations_strata read_message_async $STRATUM_INDEX
echo "XXX: Debug waiting on barrier"
wait    # wait until all incoming data are here and all outgoing topics are created - in this case we can continue

echo "Executing Souffle ${PROGRAM_NAME}"
./${PROGRAM_NAME} -i${STRATUM_INDEX}

echo "Colleting results and sending to Kafka"
iterate_outgoing_relations_strata send_message_async $STRATUM_INDEX

echo "End of ${STRATUM_INDEX}"

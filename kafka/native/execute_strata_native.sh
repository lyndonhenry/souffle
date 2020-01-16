#!/bin/bash

#
# This script execute one strata
#
# This strata waits until data are ready in Kafka topics.
# Once the data are ready, it reads them, stores them in output directory and 
# starts processing. When the execution ends, results are stored in Kafka topics
#

PROGRAM_NAME=$1
RELATION_NAME=$2

echo "Beginning of strata ${RELATION_NAME}"

source "$(dirname "$0")/kafka_api.sh"

#
#   Make sure Kafka is ready
#
wait_kafka_ready

echo "Executing Souffle ${PROGRAM_NAME}"

./${PROGRAM_NAME} -Xmetadata.broker.list=${KAFKA_HOST} -i ${RELATION_NAME} 

echo "Colleting results and sending to Kafka"

echo "End of ${RELATION_NAME}"



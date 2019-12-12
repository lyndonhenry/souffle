#!/bin/bash

#
#   Iterate all output relations for a strata
#
function iterate_output_relations_strata {
    local command="$1"
    local STRATUM_INDEX="$2"

    local PRODUCED_RELATIONS=$(echo ${JSON_DATA} | jq -r ".produced_relations_per_strata | .[\"${STRATUM_INDEX}\"]")
    local OUTPUT_RELATIONS=$(echo ${JSON_DATA} | jq -r ".output_relations_per_strata | .[\"${STRATUM_INDEX}\"]")

    # Create topics for relations
    local PRODUCED_RELATIONS_EFFECTIVE=$(echo ${PRODUCED_RELATIONS} | jq -r ".[]");
    for OUTPUT_RELATION in $(echo ${OUTPUT_RELATIONS} | jq -r ".[]"); do  
        PRODUCED_RELATIONS_EFFECTIVE=( "${PRODUCED_RELATIONS_EFFECTIVE[@]/$OUTPUT_RELATION}" ) 
        $command "${CWD}/${TEST_NAME}/${OUTPUT_RELATION}.csv" "${OUTPUT_RELATION}"
    done          

    for PRODUCED_RELATION in ${PRODUCED_RELATIONS_EFFECTIVE[@]}; do    
        $command "${CWD}/${TEST_NAME}/${PRODUCED_RELATION}.facts" "${PRODUCED_RELATION}"
    done 
}

#
#   Iterate all output and produced relations. 
#   Action to do on a relation is given by a callback
#
function iterate_output_relations {
    local command="$1"
    local JSON_DATA=$(echo $(cat ${CWD}/${TEST_NAME}/${PROGRAM_NAME}.json))

    for STRATUM_INDEX in $(echo ${JSON_DATA} | jq -r '.strata_topological_order | .[]'); do        
        iterate_output_relations_strata $command $STRATUM_INDEX
    done
}


#
#   Iterate all input relations for a strate
#
function iterate_input_relations_strata {
    local command="$1"
    local STRATUM_INDEX="$2"

    # local INPUT_RELATIONS=$(echo ${JSON_DATA} | jq -r ".input_relations_per_strata | .[\"${STRATUM_INDEX}\"]")
    local CONSUMED_RELATIONS=$(echo ${JSON_DATA} | jq -r ".consumed_relations_per_strata | .[\"${STRATUM_INDEX}\"]")

    # for INPUT_RELATION in $(echo ${INPUT_RELATIONS} | jq -r ".[]"); do
    #     echo "Input relation ${INPUT_RELATION}"
    # done       

    # Read facts from Kafka and store to the .facts file in the output dir 
    for CONSUMED_RELATION in $(echo ${CONSUMED_RELATIONS} | jq -r ".[]"); do
        $command "${CWD}/${TEST_NAME}/${CONSUMED_RELATION}.facts" "${CONSUMED_RELATION}" "$STRATUM_INDEX"
    done
}


#
#   Send message to kafka 
#
function send_message {
    local FILE=$1
    local TOPIC=$2

    local ff=$(basename ${FILE})
    echo "Sending msg from file: ${ff} to topic: ${TOPIC}"

    #   Send number of messages first
    cat "$FILE" | wc -l | kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${TOPIC}"
    #   continue with messages
    cat "$FILE" | kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${TOPIC}"
}

#
# Read message from Kafka
#
function read_message {
    local FILE=$1
    local TOPIC=$2
    local STRATUM_INDEX=$3

    echo "Reading msg from topic: ${TOPIC}, storing to file ${FILE}"

    # Note that we create a message group for the consumer named as (topic, strata_index)
    # This is for other stratas to be able to consume the same messages as well (they will belong groups named by their topics and index)
    group="${TOPIC}_${STRATUM_INDEX}"

    # get number of messages first
    size=$(kafka-console-consumer.sh --bootstrap-server ${KAFKA_HOST} --consumer-property auto.offset.reset=earliest --group ${group} --max-messages 1 --topic ${TOPIC})
    echo "Message length ${size}"
    # then get the messages
    kafka-console-consumer.sh --bootstrap-server ${KAFKA_HOST} --consumer-property auto.offset.reset=earliest --group ${group}  --max-messages ${size} --topic ${TOPIC} > $FILE
}

#
#   Create a topic
#
function create_topic() {
    local FILE=$1
    local TOPIC=$2

    echo "Creating topic: ${TOPIC}"

    kafka-topics.sh --create --bootstrap-server ${KAFKA_HOST} --replication-factor 1 --partitions 1 --topic "${TOPIC}"
}

#   
#   Delete a topic
#
function delete_topic() {
    local FILE=$1
    local TOPIC=$2

    echo "Deleting topic: ${TOPIC}"

    kafka-topics.sh --delete --bootstrap-server ${KAFKA_HOST} --topic "${TOPIC}"
}


#!/bin/bash


#
#   Iterate all stratas by topological order.
#   Action to do on a relation is given by a callback
#
function iterate_stratas {
    local command="$1"
    local subcommand="${2:-""}"

    if [[ "$subcommand" == "" ]]; then
        for STRATUM_INDEX in $(echo ${JSON_DATA} | jq -r '.strata_topological_order | .[]'); do        
            $command "$STRATUM_INDEX" 
        done
    else 
        for STRATUM_INDEX in $(echo ${JSON_DATA} | jq -r '.strata_topological_order | .[]'); do        
            $command $subcommand "$STRATUM_INDEX" 
        done
    fi

}


#
#   Iterate all output relations for a strata
#
function iterate_outgoing_relations_strata {
    local command="$1"
    local STRATUM_INDEX="$2"

    local PRODUCED_RELATIONS=$(echo ${JSON_DATA} | jq -r ".produced_relations_per_strata | .[\"${STRATUM_INDEX}\"]")
    local OUTPUT_RELATIONS=$(echo ${JSON_DATA} | jq -r ".output_relations_per_strata | .[\"${STRATUM_INDEX}\"]")

    # Create topics for relations
    local PRODUCED_RELATIONS_EFFECTIVE=$(echo ${PRODUCED_RELATIONS} | jq -r ".[]");
    for OUTPUT_RELATION in $(echo ${OUTPUT_RELATIONS} | jq -r ".[]"); do  
        PRODUCED_RELATIONS_EFFECTIVE=( "${PRODUCED_RELATIONS_EFFECTIVE[@]/$OUTPUT_RELATION}" ) 
        $command "${OUTPUT_RELATION}" "${OUTPUT_DIR}/${OUTPUT_RELATION}.csv"
    done          

    for PRODUCED_RELATION in ${PRODUCED_RELATIONS_EFFECTIVE[@]}; do    
        $command "${PRODUCED_RELATION}" "${OUTPUT_DIR}/${PRODUCED_RELATION}.facts" 
    done 
}


#
#   Iterate output relations only
#
function iterate_output_relations_strata {
    local command="$1"
    local STRATUM_INDEX="$2"

    local PRODUCED_RELATIONS=$(echo ${JSON_DATA} | jq -r ".produced_relations_per_strata | .[\"${STRATUM_INDEX}\"]")
    local OUTPUT_RELATIONS=$(echo ${JSON_DATA} | jq -r ".output_relations_per_strata | .[\"${STRATUM_INDEX}\"]")

    # Create topics for relations
    for OUTPUT_RELATION in $(echo ${OUTPUT_RELATIONS} | jq -r ".[]"); do  
        $command "${OUTPUT_RELATION}" "${OUTPUT_DIR}/${OUTPUT_RELATION}.csv"
    done          
}

#
#   Iterate all input relations for a strata
#
function iterate_input_relations_strata {
    local command="$1"
    local STRATUM_INDEX="$2"

    local INPUT_RELATIONS=$(echo ${JSON_DATA} | jq -r ".input_relations_per_strata | .[\"${STRATUM_INDEX}\"]")

    for INPUT_RELATION in $(echo ${INPUT_RELATIONS} | jq -r ".[]"); do
        $command "${INPUT_RELATION}-input" "$STRATUM_INDEX"
    done       
}

#
#   Iterate all input relations for a strata
#
function iterate_incoming_relations_strata {
    local command="$1"
    local STRATUM_INDEX="$2"

    local INPUT_RELATIONS=$(echo ${JSON_DATA} | jq -r ".input_relations_per_strata | .[\"${STRATUM_INDEX}\"]")
    local CONSUMED_RELATIONS=$(echo ${JSON_DATA} | jq -r ".consumed_relations_per_strata | .[\"${STRATUM_INDEX}\"]")

    for INPUT_RELATION in $(echo ${INPUT_RELATIONS} | jq -r ".[]"); do
        $command "${INPUT_RELATION}-input" "${STRATUM_INDEX}" "${INPUT_DIR}"
    done       

    # Read facts from Kafka and store to the .facts file in the output dir 
    for CONSUMED_RELATION in $(echo ${CONSUMED_RELATIONS} | jq -r ".[]"); do
        $command "${CONSUMED_RELATION}" "$STRATUM_INDEX" "${OUTPUT_DIR}"
    done
}


#
#   Send message to kafka 
#
function send_message {
    local TOPIC=$1
    local FILE=${2}

    local ff=$(basename ${FILE})
    echo "Sending msg from file: ${ff} to topic: ${TOPIC}"

    #   Send number of messages first
    cat "$FILE" | wc -l | kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${TOPIC}"
    #   Send file name as part of message so we know where to store the message for unmarshalling
    echo "$ff" | kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${TOPIC}"
    #   continue with messages
    cat "$FILE" | kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${TOPIC}"
}

#
# Read message from Kafka
#
function read_message {
    local TOPIC=$1
    local GROUP_NAME=$2
    local DIR=${3}

    # Note that we create a message group for the consumer named as (topic, strata_index)
    # This is for other stratas to be able to consume the same messages as well (they will belong groups named by their topics and index)
    group="${TOPIC}_${GROUP_NAME}"

    echo "Reading msg from topic: ${TOPIC}, group ${group}, storing to dir ${DIR}"

    # get number of messages first
    size=$(kafka-console-consumer.sh --bootstrap-server ${KAFKA_HOST} --consumer-property auto.offset.reset=earliest --group ${group} --max-messages 1 --topic ${TOPIC})
    file=$(kafka-console-consumer.sh --bootstrap-server ${KAFKA_HOST} --consumer-property auto.offset.reset=earliest --group ${group} --max-messages 1 --topic ${TOPIC})
    echo "Incoming message length ${size}, file ${file}"
    # then get the messages
    kafka-console-consumer.sh --bootstrap-server ${KAFKA_HOST} --consumer-property auto.offset.reset=earliest --group ${group}  --max-messages ${size} --topic ${TOPIC} > "${DIR}/${file}"
}

#
#   Create a topic
#
function create_topic() {
    local TOPIC=$1

    echo "Creating topic: ${TOPIC}"

    kafka-topics.sh --create --bootstrap-server ${KAFKA_HOST} --replication-factor 1 --partitions 1 --topic "${TOPIC}"
}

#
#   Create topic if it does not exist. It may be slow because it calls Kafka, so be careful 
#
function create_topic_if_missing {
    local TOPIC=$1

    local search=$(kafka-topics.sh --list --bootstrap-server ${KAFKA_HOST} | grep $TOPIC)
    if [[ "$search" != "$TOPIC" ]]; then
        create_topic $TOPIC
    fi

}

#   
#   Delete a topic
#
function delete_topic() {
    local TOPIC=$1

    echo "Deleting topic: ${TOPIC}"

    kafka-topics.sh --delete --bootstrap-server ${KAFKA_HOST} --topic "${TOPIC}"
}

#
#   Wait until a topic exists in Kafka
#
function wait_topic_exists {
    local TOPIC=$1

    echo "Waiting for topic:  $TOPIC"
    
    while :; do
        local search=$(kafka-topics.sh --list --bootstrap-server ${KAFKA_HOST} | grep -w $TOPIC)
        if [[ "$search" == "$TOPIC" ]]; then
            break;
        fi
        echo "XXX Debug: Topic $TOPIC not ready yet, waiting. (Discovered topics ${search})"
        sleep 5
    done
}

#
#   Check that Kafka is ready, it periodically tries to get topics
#
function wait_kafka_ready {
    local search="EMPTY TOPICS"
    while :; do
        echo "Waiting for Kafka to become ready ${search}"
        search=$(kafka-topics.sh --list --bootstrap-server ${KAFKA_HOST})
        if [[ "$search" != "EMPTY TOPICS" ]]; then
            break;
        fi
        sleep 5
    done  
    echo "Kafka ready"
}


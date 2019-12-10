#!/bin/bash

#
# //kafka/example.sh
#

#
# This script serves as an example of how to use the -e and -r options for
# Souffle.
#
# They make information available which allows each strata of a Datalog
# program to be evaluated in isolation.
#
# For each strata, this script shows how to:
#
# - get the input relations of a strata
# - get the output relations of a strata
# - get the relations produced by a strata
# - get the relations consumed by a strata
# - get a topological order over the strata
# - execute a strata as an independent subprogram
# - the locations of input, output, and intermediate files
#
# This should be enough to get Kafka working with Souffle; Each relation is a
# topic, each node is a stratum, all IO is via files.
#

set -oue pipefail

KAFKA_HOST=localhost:9092

# set -x

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



function main() {

    # if we are not running in the root souffle directory, then exit with error
    [[ $(basename ${PWD}) != souffle ]] && exit 1

    # if jq is not installed, print a helpful message
    [[ ! $(which jq) ]] && echo "Please install jq, for more information see https://stedolan.github.io/jq" && exit 1

    # compile souffle to executable at ./src/souffle
    make -j8

    # make a temporary directory for files used by this script
    CWD=${PWD}/tests/testsuite.dir
    mkdir -p ${CWD}

    # use an arbitrary test case for this example
    local TEST_NAME=example/input_output

    # if the test has not been copied to the temporary directory...
    if [ ! -e ${CWD}/${TEST_NAME} ]
    then

        # ...then copy it
        mkdir -p ${CWD}/$(dirname ${TEST_NAME})
        cp -r ${PWD}/tests/${TEST_NAME} ${CWD}/${TEST_NAME}

    fi

    # the program name is the same as the datalog program without '.dl' extension
    local PROGRAM_NAME=$(basename ${TEST_NAME})

    # if the datalog program in the test has not been compiled to C++
    if [ ! -e ${CWD}/${TEST_NAME}/${PROGRAM_NAME}.cpp ]
    then

        # ...then do so to produce a C++ program, executable, and '.json' file with program information produced as a result of the '-r' and '-e' options
        local SOUFFLE_EXE=${PWD}/src/souffle
        cd ${CWD}/${TEST_NAME}
        ${SOUFFLE_EXE} \
            -efile \
            -D${CWD}/${TEST_NAME} \
            -F${CWD}/${TEST_NAME}/facts \
            -o${PROGRAM_NAME} \
            -r${PROGRAM_NAME}.html \
            ${PROGRAM_NAME}.dl
        cd -

    fi

    iterate_output_relations create_topic

    # get the program information from the generated json file
    JSON_DATA=$(echo $(cat ${CWD}/${TEST_NAME}/${PROGRAM_NAME}.json))

    # iterate over each stratum in topological order
    for STRATUM_INDEX in $(echo ${JSON_DATA} | jq -r '.strata_topological_order | .[]')
    do

        # execute the souffle program for the current stratum...
        # - this will load input facts files from the directory passed with the -F option, these files will have a .facts extension
        # - this will store output csv files from the directory passed with the -D option, these files will have a .csv extension
        # - this will also handle intermediate relations
        # - these are relations which are produced and consumed by some stratum
        # - but are not output relations
        # - note that all input relations become such intermediate relations
        # - intermediate relations are loaded and stored from and to the output directory with a .facts extension

        # TODO - run in a thread here
        echo "Invoking strata index ${STRATUM_INDEX}"

       iterate_input_relations_strata read_message $STRATUM_INDEX

        # Invoke strata 
        ${CWD}/${TEST_NAME}/${PROGRAM_NAME} -i${STRATUM_INDEX}

        iterate_output_relations_strata send_message $STRATUM_INDEX

        # TODO - thread end here

    done

    iterate_output_relations delete_topic
}

main ${@:-}

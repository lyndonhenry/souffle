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

        # get information for each stratum from the json data
        local INPUT_RELATIONS=$(echo ${JSON_DATA} | jq -r ".input_relations_per_strata | .[\"${STRATUM_INDEX}\"]")
        local OUTPUT_RELATIONS=$(echo ${JSON_DATA} | jq -r ".output_relations_per_strata | .[\"${STRATUM_INDEX}\"]")
        local CONSUMED_RELATIONS=$(echo ${JSON_DATA} | jq -r ".consumed_relations_per_strata | .[\"${STRATUM_INDEX}\"]")
        local PRODUCED_RELATIONS=$(echo ${JSON_DATA} | jq -r ".produced_relations_per_strata | .[\"${STRATUM_INDEX}\"]")

        # print information for each stratum
        # echo "["
        # echo "STRATUM_INDEX"=${STRATUM_INDEX}
        # echo "INPUT_RELATIONS"=${INPUT_RELATIONS}
        # echo "OUTPUT_RELATIONS"=${OUTPUT_RELATIONS}
        # echo "PRODUCED_RELATIONS"=${PRODUCED_RELATIONS}
        # echo "CONSUMED_RELATIONS"=${CONSUMED_RELATIONS}
        # echo "]"

        # TODO - run in a thread here

        # Wait until all output .csv and .facts are ready
        for CONSUMED_RELATION in $(echo ${CONSUMED_RELATIONS} | jq -r ".[]"); do
            # TODO invoke in a thread here
            echo "Consumed relation ${CONSUMED_RELATION}"
        done
        for INPUT_RELATION in $(echo ${INPUT_RELATIONS} | jq -r ".[]"); do
            # TODO invoke a thread here 
            echo "Input relation ${INPUT_RELATION}"
        done        

        # Invoke strata 
        ${CWD}/${TEST_NAME}/${PROGRAM_NAME} -i${STRATUM_INDEX}

        # Send all produced (.facts) and output (.csv) relations into Kafka
        local PRODUCED_RELATIONS_EFFECTIVE=$(echo ${PRODUCED_RELATIONS} | jq -r ".[]");
        for OUTPUT_RELATION in $(echo ${OUTPUT_RELATIONS} | jq -r ".[]"); do  
            echo "Output rel: ${OUTPUT_RELATION}"  
            # kafka-topics.sh --create --bootstrap-server ${KAFKA_HOST} --replication-factor 1 --partitions 1 --topic "${OUTPUT_RELATION}"
            # PRODUCED_RELATIONS_EFFECTIVE=( "${PRODUCED_RELATIONS_EFFECTIVE[@]/$OUTPUT_RELATION}" ) 
            # # cat "${CWD}/${TEST_NAME}/${OUTPUT_RELATION}.csv" | while read line; do
            # #     kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${OUTPUT_RELATION}"
            # # done            
            # echo "Sending ${CWD}/${TEST_NAME}/${OUTPUT_RELATION}.csv"
            # cat "${CWD}/${TEST_NAME}/${OUTPUT_RELATION}.csv" | kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${OUTPUT_RELATION}"

        done          
        echo "Eff rel: ${PRODUCED_RELATIONS_EFFECTIVE}"
        for PRODUCED_RELATION in $(echo ${PRODUCED_RELATIONS_EFFECTIVE[@]} | jq -r ".[]"); do    
            echo "Produced rel: ${PRODUCED_RELATION}"
            # kafka-topics.sh --create --bootstrap-server ${KAFKA_HOST} --replication-factor 1 --partitions 1 --topic "${PRODUCED_RELATION}"
            # if [ -f "${CWD}/${TEST_NAME}/${PRODUCED_RELATION}.facts" ]; then
            #     # cat "${CWD}/${TEST_NAME}/${PRODUCED_RELATION}.facts" | while read line; do
            #     #     kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${PRODUCED_RELATION}"
            #     # done            
            #     echo "${CWD}/${TEST_NAME}/${PRODUCED_RELATION}.facts"
            #     cat "${CWD}/${TEST_NAME}/${PRODUCED_RELATION}.facts" | kafka-console-producer.sh --broker-list ${KAFKA_HOST} --topic "${PRODUCED_RELATION}"
            # fi    
        done          

        # TODO - thread end here

    done
}

main ${@:-}

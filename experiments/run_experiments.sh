#!/bin/bash

set -ouex pipefail

#
# This script can be used to run the experiments.
# Right now, it is just a starting point. 
# It is to be run from the root directory of the Souffle repository.
#

function run_example_experiment() {

  local ID="${1}"

  # ensure the output directory exists

  mkdir -p ${HOME}/.souffle/output/${ID}

  # run the NR.dl program on the "example" data set in standard interpreter mode

  ./src/souffle -F${HOME}/.souffle/input/example -D${HOME}/.souffle/output/${ID} ./experiments/NR.dl

}

function main() {

  # we first test to ensure that they system is working as expected

  # make test input and output directories
  local TEST_INPUT_DIR="${HOME}/.souffle/test/input"
  local TEST_OUTPUT_DIR="${HOME}/.souffle/test/output"

  mkdir -p ${TEST_INPUT_DIR}
  mkdir -p ${TEST_OUTPUT_DIR}

  # generate test input

  echo -e "1\t2\n2\t3\n3\t4" > "${TEST_INPUT_DIR}/E.facts"

  # test each datalog file on the test input

  local FILE
  for FILE in ./experiments/*.dl
  do
    ./src/souffle -F${TEST_INPUT_DIR} -D${TEST_OUTPUT_DIR} ${FILE}
  done

  # we are now ready to run our experiments

  # sync the data sets in the s3 bucket to the data directory at ${HOME}/.souffle

  aws s3 sync s3://souffle-on-kafka/input ${HOME}/.souffle/input

  # create a unique ID for this set of experiments

  local ID=$(date | tr ' ' '-' | tr ':' '-')

  # this part is where the experiments are invoked
  # each experiement is a separate function
  # this makes them easy to disable by commenting out
  # new experiments must be added here in the future
  # these will be determined by the research questions
  # for now, we just run an example case, passing the unique ID

  run_example_experiment ${ID}

  # finally, we sync our result set back to the s3 bucket

  aws s3 sync ${HOME}/.souffle/output s3://souffle-on-kafka/output

}

main

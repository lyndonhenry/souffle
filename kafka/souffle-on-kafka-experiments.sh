#!/bin/bash

set -oue pipefail

function _generate_first_round_of_experiments() {

  #
  # Generate the first round of experiments.
  #
  # This tests Souffle scaled with threads vs Souffle on Kafka scaled with splits.
  # Here, each Souffle on Kafka stratum is executed in one Docker.
  # The Souffle on Kafka experiments here may be run in the cloud.
  # Only one dataset is used, and one benchmark, but for both number and symbol types.
  # The total number of docker-compose.yml files is
  # (|THREADS| * |TYPES|) + (|SPLITS| * |TYPES| * |JOINS|)  = (8 * 2) + (8 * 2 * 4) = 80.
  #

  local DATASETS
  local BENCHMARKS
  local TYPES
  local THREADS
  local SPLITS
  local JOINS

  DATASETS="soc-LiveJournal1"
  BENCHMARKS="NR"
  TYPES="number symbol"
  THREADS="1 2 4 8 16 32 64 128"
  SPLITS="0"
  JOINS="none"

  # Experiments without kafka
  local BENCHMARK
  for BENCHMARK in ${BENCHMARKS}
  do
    local DATASET
    for DATASET in ${DATASETS}
    do
      local TYPE
      for TYPE in ${TYPES}
      do
        for SPLIT in ${SPLITS}
        do
          for JOIN in ${JOINS}
          do
            local THREAD
            for THREAD in ${THREADS}
            do
              ./kafka/souffle-on-kafka.sh \
              --benchmark "${BENCHMARK}" \
              --type "${TYPE}" \
              --split "${SPLIT}" \
              --join "${JOIN}" \
              --mode "no-kafka" \
              --algorithm "SNE" \
              --data "${DATASET}" \
              --threads "${THREAD}"
            done
          done
        done
      done
    done
  done

  DATASETS="soc-LiveJournal1"
  BENCHMARKS="NR"
  TYPES="number symbol"
  THREADS="1"
  SPLITS="0 2 4 8 16 32 64 128"
  JOINS="left balanced complete lattice"

  # Experiments with kafka
  local BENCHMARK
  for BENCHMARK in ${BENCHMARKS}
  do
    local DATASET
    for DATASET in ${DATASETS}
    do
      local TYPE
      for TYPE in ${TYPES}
      do
        for SPLIT in ${SPLITS}
        do
          for JOIN in ${JOINS}
          do
            local THREAD
            for THREAD in ${THREADS}
            do
              ./kafka/souffle-on-kafka.sh \
              --benchmark "${BENCHMARK}" \
              --type "${TYPE}" \
              --split "${SPLIT}" \
              --join "${JOIN}" \
              --mode "many-kafka" \
              --algorithm "GPCSNE" \
              --data "${DATASET}" \
              --threads "${THREAD}"
            done
          done
        done
      done
    done
  done

}

function _generate_example_benchmarks() {

  # Run with Kafka, using GPCSNE, on NR, with strings, two threads.

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "symbol" \
    --split "0" \
    --join "none" \
    --mode "many-kafka" \
    --algorithm "GPCSNE" \
    --data "example" \
    --threads "2"

  # Run without Kafka, using SNE, on the LR benchmark, with numbers, and one thread.

  ./kafka/souffle-on-kafka.sh \
    --benchmark "LR" \
    --type "number" \
    --split "0" \
    --join "none" \
    --mode "no-kafka" \
    --algorithm "SNE" \
    --data "example" \
    --threads "1"

  # Run without Kafka, using GSNE, on the NR benchmark, with strings, and two threads.

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "symbol" \
    --split "0" \
    --join "none" \
    --mode "no-kafka" \
    --algorithm "GSNE" \
    --data "example" \
    --threads "2"

  # Run with Kafka, using GSNE, on NR, with strings, two threads.

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "number" \
    --split "0" \
    --join "none" \
    --mode "one-kafka" \
    --algorithm "GSNE" \
    --data "example" \
    --threads "2"
}

function _run_experiments() {

  local ROOT="${HOME}/.souffle"

  echo
  echo "The experiments have now been generated to ${ROOT}/docker-compose."
  echo "Each of these docker-compose files requires AWS credentials to run."
  echo "Running the rest of this script populates them from your local config."
  echo "Would you like to proceed with running the experiments?"
  echo
  read -p "Press enter to continue..."
  echo

  aws s3 sync "${ROOT}" "s3://souffle-on-kafka"

  # Run all of the generated docker-compose.yml files.

  # get AWS credentials, passed at runtime to Docker instances via docker-compose
  AWS_ACCESS_KEY_ID="$(cat "${HOME}/.aws/credentials" | grep "aws_access_key_id" | sed 's/^.*=\s*//')"
  AWS_SECRET_ACCESS_KEY="$(cat "${HOME}/.aws/credentials" | grep "aws_secret_access_key" | sed 's/^.*=\s*//')"

  # for docker compose environment variables
cat > ${HOME}/.aws/.env << EOF
AWS_ACCESS_KEY_ID=${AWS_ACCESS_KEY_ID}
AWS_SECRET_ACCESS_KEY=${AWS_SECRET_ACCESS_KEY}
EOF
  cd ${HOME}/.aws

  local NAME
  for NAME in $(ls "${ROOT}/docker-compose")
  do
    if [ ! -e "${ROOT}/output/log/${NAME}*.log" ]
    then
      sudo docker-compose --file ${ROOT}/docker-compose/${NAME}/docker-compose.yml down --remove-orphans --rmi all
      sudo docker-compose --file ${ROOT}/docker-compose/${NAME}/docker-compose.yml up -d --remove-orphans
    fi
    # repeatedly check for log to be generated, signalling program termination
    while [ ! "$(aws s3 ls "s3://souffle-on-kafka/output/log/${NAME}")" ]
    do
      sleep 5s
    done
    aws s3 sync "s3://souffle-on-kafka/output/log" "${ROOT}/output/log"
    sudo docker-compose --file ${ROOT}/docker-compose/${NAME}/docker-compose.yml down --remove-orphans --rmi all
  done

  aws s3 sync "${ROOT}" "s3://souffle-on-kafka"

}

function _main() {

  local ROOT="${HOME}/.souffle"

  # Use --reset to remove all previous experimental runs from the s3 bucket.

  case ${1:-} in "--reset")
  for DIR in "datalog" "exe" "docker-compose" "output" "log"
  do
    rm -rf "${ROOT}/${DIR}"
    aws s3 rm --recursive "s3://souffle-on-kafka/${DIR}"
  done
  exit 0
  ;;
  esac

  # Sync S3 bucket.
  mkdir -p "${ROOT}"
  aws s3 sync "s3://souffle-on-kafka" "${ROOT}"

  # Prompt the user to delete old experiments if they want to override them.

  echo
  echo "Please delete any files in ${ROOT}/{datalog,exe,docker-compose,output,data,input} you wish to override."
  echo "These will be re-generated by this script, or ignored if they already exist."
  echo
  read -p "Press enter to continue..."
  echo

  # Pull the image from Dockerhub
  sudo docker pull lyndonmhenry/souffle-on-kafka

  # Obtain and preprocess all datasest
  cargo build --manifest-path kafka/rust/souffle_on_kafka/Cargo.toml
  kafka/rust/souffle_on_kafka/target/debug/souffle_on_kafka --datasets

  aws s3 sync "${ROOT}" "s3://souffle-on-kafka"

  # Run the souffle-on-kafka.sh script to generate experiments, as desired.
  case ${1:-} in
  "--example")
    echo
    echo
    echo
    read -p "GENERATE EXAMPLE EXPERIMENTS?" &&  _generate_example_benchmarks
    echo
    echo
    echo
    read -p "RUN FIRST ROUND OF EXPERIMENTS?" && _run_experiments
    echo
    echo
    echo
  ;;
  *)
    echo
    echo
    echo
    read -p "GENERATE FIRST ROUND OF EXPERIMENTS?" && _generate_first_round_of_experiments
    echo
    echo
    echo
    read -p "RUN FIRST ROUND OF EXPERIMENTS?" && _run_experiments
    echo
    echo
    echo
    read -p "GENERATE SECOND ROUND OF EXPERIMENTS?" && _generate_second_round_of_experiments
    echo
    echo
    echo
    read -p "RUN SECOND ROUND OF EXPERIMENTS?" && _run_experiments
    echo
    echo
    echo
  ;;
  esac



}

_main ${@:-}
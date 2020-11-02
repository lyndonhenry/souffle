#!/bin/bash

set -ouex pipefail


function _generate_example_benchmarks() {

  # Run without Kafka, using SNE, on NR, with numbers, 1 thread, no splits or joins.

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "number" \
    --split "0" \
    --join "none" \
    --mode "no-kafka" \
    --algorithm "SNE" \
    --data "complete-graph-4" \
    --threads "1" \
    --subdir "example"

  # Run with Kafka, in one Docker, using GPCSNE, on ALL, with symbols, 2 threads, no splits or joins.

  ./kafka/souffle-on-kafka.sh \
    --benchmark "ALL" \
    --type "symbol" \
    --split "0" \
    --join "none" \
    --mode "one-kafka" \
    --algorithm "GPCSNE" \
    --data "example" \
    --threads "2" \
    --subdir "example"


  # Run with Kafka, in many Dockers, using GPCSNE, on ALL, with symbols, 2 threads, no splits no joins.

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "symbol" \
    --split "0" \
    --join "none" \
    --mode "many-kafka" \
    --algorithm "GPCSNE" \
    --data "example" \
    --threads "2" \
    --subdir "example"

  # Run with Kafka, in many Dockers, using GPCSNE, on NR, with symbols, 1 thread, 2 splits, and complete join.

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "symbol" \
    --split "2" \
    --join "complete" \
    --mode "many-kafka" \
    --algorithm "GPCSNE" \
    --data "example" \
    --threads "1" \
    --subdir "example"

}

function _generate_first_round_of_experiments() {

  #
  # Generate the first round of experiments.
  #
  # This tests Souffle scaled with threads vs Souffle on Kafka scaled with splits.
  # Here, each Souffle on Kafka stratum is executed in one Docker each.
  # The Souffle on Kafka experiments here may be run in the cloud.
  # The dataset used is the complete graph with weak scaling, starting at 2 ** 7 vertices.
  # The total number of docker-compose.yml files is
  # (|THREADS| * |TYPES|) + (|SPLITS| * |TYPES| * |JOINS|)  = (7 * 2) + (7 * 2 * 4) = 70.
  #

  local DATASETS
  local BENCHMARKS
  local TYPES
  local THREADS
  local SPLITS
  local JOINS
  local SUBDIR

  # @TODO (lh): this has been changed, it used to be complete-graph, need to ensure half-complete graph is ok
  DATASETS="half-complete-graph"
  BENCHMARKS="NR"
  TYPES="number symbol"

  THREADS="1 2 4 8 16 32"
  SPLITS="0"
  JOINS="none"
  SUBDIR="no-cloud"

  local COUNT

  COUNT=7

  # Experiments without kafka

  local THREAD
  for THREAD in ${THREADS}
  do
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
              local SIZE=$((2 ** COUNT))
              ./kafka/souffle-on-kafka.sh \
              --benchmark "${BENCHMARK}" \
              --type "${TYPE}" \
              --split "${SPLIT}" \
              --join "${JOIN}" \
              --mode "no-kafka" \
              --algorithm "SNE" \
              --data "${DATASET}-${SIZE}" \
              --threads "${THREAD}" \
              --subdir "${SUBDIR}"
            done
          done
        done
      done
    done
    ((COUNT++))
  done

  THREADS="1"
  SPLITS="1 2 4 8 16 32"
  JOINS="complete left balanced"
  SUBDIR="yes-cloud"

  COUNT=7

  # Experiments with kafka
  for SPLIT in ${SPLITS}
  do
    local BENCHMARK
    for BENCHMARK in ${BENCHMARKS}
    do
      local DATASET
      for DATASET in ${DATASETS}
      do
        local TYPE
        for TYPE in ${TYPES}
        do
          for JOIN in ${JOINS}
          do
            local THREAD
            for THREAD in ${THREADS}
            do
              local SIZE=$((2 ** COUNT))
              ./kafka/souffle-on-kafka.sh \
              --benchmark "${BENCHMARK}" \
              --type "${TYPE}" \
              --split "${SPLIT}" \
              --join "${JOIN}" \
              --mode "many-kafka" \
              --algorithm "GPCSNE" \
              --data "${DATASET}-${SIZE}" \
              --threads "${THREAD}" \
              --subdir "${SUBDIR}"
            done
          done
        done
      done
    done
    ((COUNT++))
  done

}

function _generate_second_round_of_experiments() {
  #
  # Generate the second round of experiments.
  #
  # This tests Souffle with Kafka vs Souffle with no Kafka.
  # Here, all Souffle strata are executed together in the same Docker.
  # These experiments are not required to be run in the cloud.
  # All datasets are used, for both number and symbol types.
  # Only the ALL benchmark is used, containing all other benchmarks.
  # Eight threads is used, with no splits or joins.
  # The total number of docker-compose.yml files is
  # (|DATASETS| * |TYPES|) * 2  = (18 * 2) * 2 = 72
  #

  local DATASETS
  local BENCHMARKS
  local TYPES
  local THREADS
  local SPLITS
  local JOINS
  local SUBDIR

  DATASETS+="cit-Patents "
  DATASETS+="com-Orkut "
  DATASETS+="com-Youtube "
  DATASETS+="roadNet-CA "
  DATASETS+="roadNet-PA "
  DATASETS+="roadNet-TX "
  DATASETS+="soc-LiveJournal1 "
  DATASETS+="soc-Epinions1 "
  DATASETS+="soc-Pokec "
  DATASETS+="web-BerkStan "
  DATASETS+="web-Google "
  DATASETS+="web-NotreDame "
  DATASETS+="web-Stanford "
  DATASETS+="wiki-Talk "
  DATASETS+="wiki-topcats "
  DATASETS+="prog-jenkins "
  DATASETS+="prog-jython "
  DATASETS+="prog-openjdk8 "

  BENCHMARKS="ALL"
  TYPES="number symbol"
  THREADS="6"
  SPLITS="0"
  JOINS="none"

  SUBDIR="no-cloud"

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
              --threads "${THREAD}" \
              --subdir "${SUBDIR}"
            done
          done
        done
      done
    done
  done

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
              --mode "one-kafka" \
              --algorithm "GPCSNE" \
              --data "${DATASET}" \
              --threads "${THREAD}" \
              --subdir "${SUBDIR}"
            done
          done
        done
      done
    done
  done

}

function _main() {

  local ROOT="${HOME}/.souffle"

  # Sync S3 bucket.
  rm -rf "${ROOT}/{exe,datalog,docker-compose}"
  mkdir -p "${ROOT}"
  aws s3 sync "s3://souffle-on-kafka" "${ROOT}" || :

  _generate_example_benchmarks

  echo
  read -p "Example experiments generated, press enter to generate the real experiments..."
  echo

  _generate_first_round_of_experiments

  _generate_second_round_of_experiments

}

_main
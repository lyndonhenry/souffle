#!/bin/bash

set -ouex pipefail

function _generate_yes_cloud_experiments() {

  #
  # Generate the first round of experiments.
  #
  # This tests Souffle scaled with threads vs Souffle on Kafka scaled with splits.
  # Here, each Souffle on Kafka stratum is executed in one Docker each.
  # The Souffle on Kafka experiments here may be run in the cloud.
  # Only one dataset is used, and one benchmark, but for both number and symbol types.
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

  DATASETS="soc-LiveJournal1"
  BENCHMARKS="NR"
  TYPES="number symbol"

  THREADS="1 2 4 8 16 32 64"
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

  THREADS="1"
  SPLITS="0 2 4 8 16 32 64"
  JOINS="left balanced complete lattice"
  SUBDIR="yes-cloud"

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
              --threads "${THREAD}" \
              --subdir "${SUBDIR}"
            done
          done
        done
      done
    done
  done

}

function _generate_no_cloud_experiments() {

  #
  # Generate the second round of experiments.
  #
  # This tests Souffle with Kafka vs Souffle with no Kafka.
  # Here, all Souffle strata are executed together in the same Docker.
  # These experiments are not required to be run in the cloud.
  # All datasets and benchmarks are used, for both number and symbol types.
  # Only one thread is used, with no splits or joins.
  # The total number of docker-compose.yml files is
  # (|DATASETS| * |BENCHMARKS| * |TYPES|) * 2  = (18 * 8 * 2) * 2 = 576
  #

  local DATASETS
  local BENCHMARKS
  local TYPES
  local THREADS
  local SPLITS
  local JOINS
  local SUBDIR

  DATASETS+="cit-Patents"
  DATASETS+="com-Orkut"
  DATASETS+="com-Youtube"
  DATASETS+="roadNet-CA"
  DATASETS+="roadNet-PA"
  DATASETS+="roadNet-TX"
  DATASETS+="soc-LiveJournal1"
  DATASETS+="soc-Epinions1"
  DATASETS+="soc-Pokec"
  DATASETS+="web-BerkStan"
  DATASETS+="web-Google"
  DATASETS+="web-NotreDame"
  DATASETS+="web-Stanford"
  DATASETS+="wiki-Talk"
  DATASETS+="wiki-topcats"
  DATASETS+="prog-jenkins"
  DATASETS+="prog-jython"
  DATASETS+="prog-openjdk8"

  BENCHMARKS+="LR"
  BENCHMARKS+="RR"
  BENCHMARKS+="NR"
  BENCHMARKS+="SG"
  BENCHMARKS+="RSG"
  BENCHMARKS+="TC"
  BENCHMARKS+="SCC"
  BENCHMARKS+="MN"

  TYPES="number symbol"
  THREADS="1"
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
    --threads "2" \
    --subdir "example"

  # Run without Kafka, using SNE, on the NR benchmark, with numbers, one thread, splits and joins

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "number" \
    --split "4" \
    --join "lattice" \
    --mode "no-kafka" \
    --algorithm "SNE" \
    --data "example" \
    --threads "1" \
    --subdir "example"

  # Run with Kafka, using SNE, on the NR benchmark, with numbers, one thread, splits and joins

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "number" \
    --split "4" \
    --join "complete" \
    --mode "many-kafka" \
    --algorithm "SNE" \
    --data "example" \
    --threads "1" \
    --subdir "example"

}

function _main() {

  local ROOT="${HOME}/.souffle"

  # Sync S3 bucket.
  mkdir -p "${ROOT}"
  aws s3 sync "s3://souffle-on-kafka" "${ROOT}"

  _generate_example_benchmarks
  aws s3 sync "${ROOT}" "s3://souffle-on-kafka"

  echo
  read -p "Example experiments generated, press enter to generate the real experiments..."
  echo

  _generate_yes_cloud_experiments
  _generate_no_cloud_experiments

  aws s3 sync "${ROOT}" "s3://souffle-on-kafka"

}

_main
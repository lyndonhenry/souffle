#!/bin/bash

set -ouex pipefail


function _generate_example_benchmarks() {

  ./kafka/souffle-on-kafka.sh \
    --benchmark "ALL" \
    --type "number" \
    --split "1" \
    --join "none" \
    --mode "no-kafka" \
    --algorithm "SNE" \
    --data "example" \
    --threads "1" \
    --subdir "example"

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "symbol" \
    --split "2" \
    --join "complete" \
    --mode "no-kafka" \
    --algorithm "SNE" \
    --data "example"
    --threads "2" \
    --subdir "example"

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "number" \
    --split "2" \
    --join "complete" \
    --mode "one-kafka" \
    --algorithm "GPCSNE" \
    --data "example" \
    --threads "2" \
    --subdir "example"

  ./kafka/souffle-on-kafka.sh \
    --benchmark "NR" \
    --type "symbol" \
    --split "2" \
    --join "complete" \
    --mode "many-kafka" \
    --algorithm "GPCSNE" \
    --data "example" \
    --threads "2" \
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
  # (|THREADS| * |TYPES|) + (|SPLITS| * |TYPES| * |JOINS|)  = (7 * 2) + (7 * 2 * 4 * 2) = .
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
  SUBDIR="first/no-cloud"

  local COUNT

  COUNT=6

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

  COUNT=6
  SUBDIR="first/no-cloud"

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
              --mode "one-kafka" \
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

  COUNT=6
  SUBDIR="first/no-cloud"

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

  THREADS="1"
  SPLITS="1 2 4 8 16 32 64"
  JOINS="complete"

  COUNT=6
  SUBDIR="first/yes-cloud"

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

function _generate_third_round_of_experiments() {

  # This experiment is to see if there is a sweet spot between number of splits and number of threads.
  # We keep the size of the data constant and increase the number of threads and splits.

  # 4 + (2 * SPLITS) = TOTAL STRATA
  # TOTAL STRATA * THREADS = TOTAL THREADS
  # 2t,8s = 20 * 2 = 40
  # 4t,4s = 12 * 4 = 48
  # 8t,2s = 8 * 8 = 64
  DATASETS="half-complete-graph"
  BENCHMARKS="NR"
  TYPES="number symbol"
  THREADS="2 4 8"
  SPLITS="8 4 2"
  JOINS="complete"
  SUBDIR="third/no-cloud"

  COUNT=11

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
  done

  # Experiments with no kafka
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
              --mode "no-kafka" \
              --algorithm "GPCSNE" \
              --data "${DATASET}-${SIZE}" \
              --threads "${THREAD}" \
              --subdir "${SUBDIR}"
            done
          done
        done
      done
    done
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
  # (|DATASETS| * |TYPES|) * 2  = (23 * 2) * 2 = 92
  #

  local DATASETS
  local BENCHMARKS
  local TYPES
  local THREADS
  local SPLITS
  local JOINS
  local SUBDIR

  DATASETS+=" cit-HepPh "
  DATASETS+=" cit-HepTh "
  DATASETS+=" com-Amazon "
  DATASETS+=" com-DBLP "
  DATASETS+=" com-Youtube "
  DATASETS+=" ego-Facebook "
  DATASETS+=" ego-Twitter "
  DATASETS+=" email-Enron "
  DATASETS+=" email-EuAll "
  DATASETS+=" loc-Brightkite "
  DATASETS+=" loc-Gowalla "
  DATASETS+=" roadNet-CA "
  DATASETS+=" roadNet-PA "
  DATASETS+=" roadNet-TX "
  DATASETS+=" soc-Epinions1 "
  DATASETS+=" soc-Slashdot0811 "
  DATASETS+=" soc-Slashdot0902 "
  DATASETS+=" sx-askubuntu "
  DATASETS+=" sx-mathoverflow "
  DATASETS+=" sx-superuser "
  DATASETS+=" web-NotreDame "
  DATASETS+=" web-Stanford "
  DATASETS+=" web-flickr "
  DATASETS+=" wiki-Talk "
  DATASETS+=" wiki-Vote "

  BENCHMARKS="ALL"
  TYPES="number symbol"
  THREADS="16"
  SPLITS="0"
  JOINS="none"

  SUBDIR="second/no-cloud"

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

  _generate_third_round_of_experiments

}

_main

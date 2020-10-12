#!/bin/bash

# @@@TODO (lh): draft the real experiments here...

function _first_test_bach_of_experiments() {

  # This test is for...
  # - with kafka and GPCSNE vs no kafka and SNE
  # - no splits or joins
  # - all datasets
  # - all benchmark programs
  # - with and without strings
  # - with 1 thread or n threads (or n/stratum threads)

  local BENCHMARKS="LR RR NR SG RSG TC SCC MN"
  local DATATYPES="number symbol"
  local DATASETS="???" # @@@TODO (lh): do with real datasets
  local THREADS="1 1/???" # @@@TODO (lh): do with n threads or m per stratum

  # test experiments without kafka
  local BENCHMARK
  for BENCHMARK in ${BENCHMARKS}
  do
    local DATATYPE
    for DATATYPE in ${DATATYPES}
    do
      ./kafka/souffle-on-kafka.sh \
      --benchmark "${BENCHMARK}" \
      --type "${DATATYPE}" \
      --split "0" \
      --join "none" \
      --mode "no-kafka" \
      --algorithm "SNE" \
      --data "example" \
      --threads "1"
    done
  done

  # test experiments with kafka
  local BENCHMARK
  for BENCHMARK in ${BENCHMARKS}
  do
    local DATATYPE
    for DATATYPE in ${DATATYPES}
    do
      ./kafka/souffle-on-kafka.sh \
      --benchmark "${BENCHMARK}" \
      --type "${DATATYPE}" \
      --split "0" \
      --join "none" \
      --mode "one-kafka" \
      --algorithm "GPCSNE" \
      --data "example" \
      --threads "1"
    done
  done

}

function _second_test_batch_of_experiments() {

  # This test is for...
  # - with kafka and GPCSNE
  # - with splits and joins
  # - with splits scaled (how? doubling?)
  # - with each join
  # - one (or few) datasets
  # - one benchmark programs
  # - with and without strings
  # - with 1 thread or n/stratum threads

  local BENCHMARKS="LR RR NR SG RSG TC SCC MN"
  local DATATYPES="number symbol"
  local DATASETS="???" # @@@TODO (lh): do with real dataset
  local THREADS="1 1/???" # @@@TODO (lh): do with n threads or m per stratum
  local SPLITS="0 2 4 8"
  local JOINS="left lattice balanced complete"

  # test experiments without kafka
  local BENCHMARK
  for BENCHMARK in ${BENCHMARKS}
  do
    local DATATYPE
    for DATATYPE in ${DATATYPES}
    do
      local SPLIT
      for SPLIT in ${SPLITS}
      do
        local JOIN
        for JOIN in ${JOINS}
        do
          ./kafka/souffle-on-kafka.sh \
          --benchmark "${BENCHMARK}" \
          --type "${DATATYPE}" \
          --split "${SPLIT}" \
          --join "${JOIN}" \
          --mode "many-kafka" \
          --algorithm "GPCSNE" \
          --data "example" \
          --threads "1"
        done
      done
    done
  done

}

function _main() {
_first_batch_test
_second_batch_test
}

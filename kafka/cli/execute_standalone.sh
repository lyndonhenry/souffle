#!/bin/bash

#
#   Execute standard Souffle in single thread.
#   This is good for comparison with Kafka version
#

N_THREAD="${1:-"1"}"

echo "Number of threads ${N_THREAD}"

START=$(date +%s.%N)

./program -j $N_THREAD

END=$(date +%s.%N)

echo "Output dir content:"
ls -lh output

DIFF=$(echo "$END - $START" | bc)
echo "Total time: $DIFF (Sec.nanos) "


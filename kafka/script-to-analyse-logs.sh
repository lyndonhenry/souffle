#!/bin/bash

INPUT="${HOME}/.souffle/output/log/"
OUTPUT="metrics.csv"

java -jar kafka/log-parser/target/logparser-1.0-SNAPSHOT-shaded.jar $INPUT $OUTPUT
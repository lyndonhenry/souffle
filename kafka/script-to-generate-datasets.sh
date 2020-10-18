#!/bin/bash

set -oue pipefail

function _main() {

  local ROOT="${HOME}/.souffle"

  # Sync S3 bucket to local
  mkdir -p "${ROOT}"
  aws s3 sync "s3://souffle-on-kafka" "${ROOT}"

  # Obtain and preprocess all datasest
  cargo build --manifest-path kafka/rust/souffle_on_kafka/Cargo.toml
  kafka/rust/souffle_on_kafka/target/debug/souffle_on_kafka --datasets

  # Sync local to S3 bucket
  aws s3 sync "${ROOT}" "s3://souffle-on-kafka"

}

_main
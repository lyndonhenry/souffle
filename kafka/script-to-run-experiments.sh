#!/bin/bash

set -oue pipefail

# get AWS credentials, passed at runtime to Docker instances
AWS_ACCESS_KEY_ID="$(cat "${HOME}/.aws/credentials" | grep "aws_access_key_id" | sed 's/^.*=\s*//')"
AWS_SECRET_ACCESS_KEY="$(cat "${HOME}/.aws/credentials" | grep "aws_secret_access_key" | sed 's/^.*=\s*//')"

function _run_experiment() {

  local DOCKER_COMPOSE_FILE="${1}"
  local NAME="$(basename $(dirname ${DOCKER_COMPOSE_FILE}))"
  local SUBDIR="$(basename $(dirname $(dirname ${DOCKER_COMPOSE_FILE})))"
  local STACK_NAME="X$(date +%s)"
  # if the experiment has not yet run
  if [ ! "$(aws s3 ls "s3://souffle-on-kafka/output/log/${NAME}")" ]
  then
    # put the docker stack up
    # TODO (lh): use something like 'sudo docker stack deploy -c ${DOCKER_COMPOSE_FILE} ${STACK_NAME}'
    # TODO (lh): ensure AWS credentials are passed to docker stack command, e.g. via env AWS_ACCESS_KEY_ID=${AWS_ACCESS_KEY_ID} AWS_SECRET_ACCESS_KEY=${AWS_SECRET_ACCESS_KEY}
    sudo docker-compose --file ${DOCKER_COMPOSE_FILE} up -d --remove-orphans
  fi
  # repeatedly check for log to be generated, signalling program termination
  while [ ! "$(aws s3 ls "s3://souffle-on-kafka/output/log/${NAME}")" ]
  do
    echo "Waiting..."
    sleep 5s
  done
  # kill the docker stack
  # TODO (lh): use something like sudo docker stack rm ${STACK_NAME}
  sudo docker-compose --file ${DOCKER_COMPOSE_FILE} down --remove-orphans --rmi all
  # sync the log file
  aws s3 sync "s3://souffle-on-kafka/output/log" "${ROOT}/output/log"
}

function _main() {

  local ROOT="${HOME}/.souffle"

  # Sync S3 bucket.
  mkdir -p "${ROOT}"
  aws s3 sync "s3://souffle-on-kafka" "${ROOT}"

  # sync the contents of the s3 bucket locally
  aws s3 sync "${ROOT}" "s3://souffle-on-kafka"


  # TODO (lh): ensure these are passed when using 'docker stack', they populate environment variables in the docker-compose.yml file
  # for docker compose environment variables
cat > ${HOME}/.aws/.env << EOF
AWS_ACCESS_KEY_ID=${AWS_ACCESS_KEY_ID}
AWS_SECRET_ACCESS_KEY=${AWS_SECRET_ACCESS_KEY}
EOF

  # TODO (lh): possibly remove this when using 'docker stack', it is only used to pass aws credentials via a .env file, used automatically by docker-compose
  # docker will use the .env file of the current working directory to pass the aws credentials
  cd ${HOME}/.aws

  # TODO (lh): make sure the docker swarm is initialised correctly here, it is required for use of docker stack
  # initialise the docker swarm, to use the stack deploy commands
  sudo docker swarm init || :

  # run the example experiments first, to ensure everything is working as exepected
  local DOCKER_COMPOSE_FILE
  for DOCKER_COMPOSE_FILE in $(find "${ROOT}/docker-compose/example" -name "*.yml")
  do
    _run_experiment "${DOCKER_COMPOSE_FILE}"
  done

  read -p "Example experiments have run, press enter to generate the real experiments..."

  # run the cloud experiments
  local DOCKER_COMPOSE_FILE
  for DOCKER_COMPOSE_FILE in $(find "${ROOT}/docker-compose/yes-cloud" -name "*.yml")
  do
    _run_experiment "${DOCKER_COMPOSE_FILE}"
  done

  # run the non-cloud experiments
  local DOCKER_COMPOSE_FILE
  for DOCKER_COMPOSE_FILE in $(find "${ROOT}/docker-compose/no-cloud" -name "*.yml")
  do
    _run_experiment "${DOCKER_COMPOSE_FILE}"
  done

  # sync the s3 bucket
  aws s3 sync "${ROOT}" "s3://souffle-on-kafka"


}

_main ${@:-}
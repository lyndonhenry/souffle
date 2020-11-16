#!/bin/bash

set -ouex pipefail

# get AWS credentials, passed at runtime to Docker instances
AWS_ACCESS_KEY_ID="$(cat "${HOME}/.aws/credentials" | grep "aws_access_key_id" | sed 's/^.*=\s*//')"
AWS_SECRET_ACCESS_KEY="$(cat "${HOME}/.aws/credentials" | grep "aws_secret_access_key" | sed 's/^.*=\s*//')"

TIMEOUTS_FILE="timeouts.txt"

function _run_experiment() {

  local DOCKER_COMPOSE_FILE="${1}"
  local NAME="$(basename $(dirname ${DOCKER_COMPOSE_FILE}))"
  local SUBDIR="$(basename $(dirname $(dirname ${DOCKER_COMPOSE_FILE})))"
  local STACK_NAME="X$(date +%s)"
  # if the experiment has not yet run
  if [ ! "$(aws s3 ls "s3://souffle-on-kafka/output/log/${NAME}")" ]
  then

    # create a secret docker compose file, with the AWS credentials already populated
    local SECRET_DOCKER_COMPOSE_FILE="${HOME}/.secret/docker-compose.yml"
    mkdir -p $(dirname ${SECRET_DOCKER_COMPOSE_FILE})
    local AWS_SECRET_ACCESS_KEY_REGEX="$(echo "${AWS_SECRET_ACCESS_KEY}" | sed 's/\//\\\//g')"
    cat ${DOCKER_COMPOSE_FILE} | \
     sed "s/\${AWS_ACCESS_KEY_ID}/${AWS_ACCESS_KEY_ID}/g" | \
     sed "s/\${AWS_SECRET_ACCESS_KEY}/${AWS_SECRET_ACCESS_KEY_REGEX}/g" > ${SECRET_DOCKER_COMPOSE_FILE}

    # deploy the stack
    docker stack deploy -c ${SECRET_DOCKER_COMPOSE_FILE} ${STACK_NAME}

    # set a timeout of 1 hour via max count
    local COUNT=0
    local MAX_COUNT=$(((60 * 60) / 5))
    # repeatedly check for log to be generated, signalling program termination
    while [ ! "$(aws s3 ls "s3://souffle-on-kafka/output/log/${NAME}")" ]
    do
      echo "Waiting..."
      sleep 5s
      if [ "${COUNT}" -ge "${MAX_COUNT}" ]
      then
        echo "ERROR: THE EXPERIMENT HAS REACHED THE TIMEOUT!"
        echo "$NAME" >> $TIMEOUTS_FILE
        break
      fi
      COUNT=$((COUNT+1))
    done
    # kill the docker stack
    docker stack rm ${STACK_NAME}

    docker stop $(docker ps -a -q) || :
    docker rm $(docker ps -a -q) || :
    docker rmi $(docker images -q -a) || :
    docker volume rm $(docker volume ls -qf dangling=true) || :
    docker system prune -a -f

    # sync the log file
    aws s3 sync "s3://souffle-on-kafka/output/log" "${ROOT}/output/log" || :
    # give bucket owner output and log file access
    local LOG_FILES="$(aws s3 ls s3://souffle-on-kafka/output/log --recursive | awk '{print $4}')"
    local LOG_FILE
    for LOG_FILE in ${LOG_FILES}
    do
      # Run in background as it is quite slow 
      aws s3api put-object-acl --bucket souffle-on-kafka --key ${LOG_FILE} --acl bucket-owner-full-control &
    done
  fi
}

function _main() {

  local ROOT="${HOME}/.souffle"

  # Sync S3 bucket.
  mkdir -p "${ROOT}"
  aws s3 sync "s3://souffle-on-kafka" "${ROOT}" || :

  # sync the contents of the s3 bucket locally
  #aws s3 sync "${ROOT}" "s3://souffle-on-kafka"

  # initialise the docker swarm, to use the stack deploy commands
  #docker swarm init || :

  rm $TIMEOUTS_FILE || :

  # run the example experiments first, to ensure everything is working as exepected
  local DOCKER_COMPOSE_FILE
  for DOCKER_COMPOSE_FILE in $(find "${ROOT}/docker-compose/example" -name "*.yml")
  do
    _run_experiment "${DOCKER_COMPOSE_FILE}"
  done

  read -p "Example experiments have run, press enter to run the real experiments..."

  # run all experiments in the second round
  local DOCKER_COMPOSE_FILE
  for DOCKER_COMPOSE_FILE in $(find "${ROOT}/docker-compose/third/no-cloud" -name "*.yml")
  do
    _run_experiment "${DOCKER_COMPOSE_FILE}"
  done

  # run all experiments in the second round
  local DOCKER_COMPOSE_FILE
  for DOCKER_COMPOSE_FILE in $(find "${ROOT}/docker-compose/second/no-cloud" -name "*.yml")
  do
    _run_experiment "${DOCKER_COMPOSE_FILE}"
  done

  # run the non-cloud experiments in the first round
  local DOCKER_COMPOSE_FILE
  for DOCKER_COMPOSE_FILE in $(find "${ROOT}/docker-compose/first/no-cloud" -name "*.yml")
  do
    _run_experiment "${DOCKER_COMPOSE_FILE}"
  done

  # sync the s3 bucket
  #aws s3 sync "${ROOT}" "s3://souffle-on-kafka"


}

_main ${@:-}
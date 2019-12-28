#!/bin/bash

set -ouex

function ensure_kafka_topic_created() {
    # @TODO: check for existence
    local KAFKA_HOST="${1}"
    local TOPIC="${2}"
    kafka-topics.sh --create --bootstrap-server "${KAFKA_HOST}" --topic "${TOPIC}" --replication-factor 1 --partitions 1  || :
}

function ensure_kafka_topic_deleted() {
    # @TODO: check for existence
    local KAFKA_HOST="${1}"
    local TOPIC="${2}"
    kafka-topics.sh --delete --bootstrap-server "${KAFKA_HOST}" --topic "${TOPIC}" || :
}

function for_each() {
    local f="${1}"
    local xs="${@:2}"
    local x
    for x in ${xs}
    do
        ${f} ${x}
    done
}

function for_each_async() {
    local f="${1}"
    local xs="${@:2}"
    local x
    for x in ${xs}
    do
        ${f} ${x} &
    done
}

function ensure_autoreconf_is_called() {
    local CWD="${1}"
    local AUTORECONF_FLAGS="${2}"
    cd "${CWD}"
    autoreconf "${AUTORECONF_FLAGS}"
    cd -
}

function ensure_autoconf_project_is_built() {
    local CWD="${1}"
    local CONFIGURE_FLAGS="${2:-}"
    local MAKE_FLAGS="-j8"
    cd "${CWD}"
    ./configure "${CONFIGURE_FLAGS}"
    make "${MAKE_FLAGS}"
    cd -
}

function ensure_autoconf_project_is_installed() {
    local CWD="${1}"
    local CONFIGURE_FLAGS="${2:-}"
    ensure_autoconf_project_is_built "${CWD}" "${CONFIGURE_FLAGS}"
    cd "${CWD}"
    sudo make install
    sudo ldconfig
    cd -
}

function ensure_docker_compose_is_up() {
    local CWD="${1}"
    cd "${CWD}"
    sudo docker-compose up -d
    cd -
}

function ensure_docker_compose_is_down() {
    local CWD="${1}"
    cd "${CWD}"
    sudo docker-compose down
    cd -
}

function ensure_github_repo_is_cloned() {
    local CWD="${1}"
    local GITHUB_REPO="${2}"
    if [ ! -e "${CWD}/${GITHUB_REPO}" ]
    then
        git clone --recurse-submodules https://github.com/${GITHUB_REPO}.git "${CWD}/${GITHUB_REPO}"
    fi
}

function ensure_github_repo_is_updated() {
    local CWD="${1}"
    local GITHUB_REPO="${2}"
    cd "${CWD}/${GITHUB_REPO}"
    git submodule update --init --recursive
    cd -
}

function ensure_github_repo_is_cloned_and_updated() {
    local CWD="${1}"
    local GITHUB_REPO="${2}"
    ensure_github_repo_is_cloned "${CWD}" "${GITHUB_REPO}"
    ensure_github_repo_is_updated "${CWD}" "${GITHUB_REPO}"
}

function ensure_file_is_downloaded() {
    local CWD="${1}"
    local URL="${2}"
    if [ ! -e "${CWD}/$(basename "${URL}")" ]
    then
        cd "${CWD}"
        curl -O "${URL}"
        cd -
    fi
}

function ensure_tarball_is_unzipped() {
    local DIRNAME="$(dirname ${1})"
    local BASENAME="$(basename ${1})"
    if [ ! -e "${DIRNAME}/${BASENAME%%.*}" ]
    then
        cd "${DIRNAME}"
        tar -xzf "${BASENAME}"
        cd -
    fi
}

function ensure_tarball_is_downloaded_and_unzipped() {
    local CWD="${1}"
    local URL="${2}"
    ensure_file_is_downloaded "${CWD}" "${URL}"
    ensure_tarball_is_unzipped "${CWD}/$(basename ${URL})"
}

function ensure_bootstrap_is_called() {
    local CWD="${1}"
    cd "${CWD}"
    ./bootstrap
    cd -
}

function ensure_directory_exists() {
    local DIRECTORY="${1}"
    mkdir -p "${DIRECTORY}"
}

function ensure_file_is_moved() {
    local FROM="${1}"
    local TO="${2}"
    if [ ! -e "${TO}" ]
    then
        ensure_directory_exists "$(dirname "${TO}")"
        mv "${FROM}" "${TO}"
    fi
}

function ensure_file_is_copied() {
    local FROM="${1}"
    local TO="${2}"
    if [ ! -e "${TO}" ]
    then
        ensure_directory_exists "$(dirname "${TO}")"
        cp -r "${FROM}" "${TO}"
    fi
}

function ensure_apt_package_installed() {
    local PACKAGE="${1}"
    if [ ! $(which "${PACKAGE}") ]
    then
        sudo apt-get install -y "${PACKAGE}"
    fi
}

function ensure_librdkafka_is_installed() {
    local CWD="${1}"
    local GITHUB_REPO="edenhill/librdkafka"
    if [ ! -e "${CWD}/${GITHUB_REPO}/src/librdkafka.a" ]
    then
        ensure_github_repo_is_cloned "${CWD}" "${GITHUB_REPO}"
        ensure_autoconf_project_is_installed "${CWD}/${GITHUB_REPO}" "--install-deps"
    fi
}

function ensure_kafkacat_is_installed() {
    ensure_apt_package_installed kafkacat
}

function ensure_apache_kafka_is_installed() {
    local CWD="${1}"
    local APACHE_KAFKA_URL="http://mirror.vorboss.net/apache/kafka/2.3.1/kafka_2.12-2.3.1.tgz"
    ensure_tarball_is_downloaded_and_unzipped "${CWD}" "${APACHE_KAFKA_URL}"
}

function ensure_jq_is_installed() {
    local CWD="${1}"
    local GITHUB_REPO="stedolan/jq"
    local AUTORECONF_FLAGS="-fi"
    if [ ! $(which jq) ]
    then
        ensure_github_repo_is_cloned_and_updated "${CWD}" "${GITHUB_REPO}"
        ensure_autoreconf_is_called "${CWD}/${GITHUB_REPO}" "${AUTORECONF_FLAGS}"
        ensure_autoconf_project_is_installed "${CWD}/${GITHUB_REPO}"
    fi
}

function ensure_souffle_is_built_for_kafka() {
    local CWD="${1}"
    ensure_bootstrap_is_called "${CWD}"
    ensure_autoconf_project_is_built "${CWD}" "--enable-kafka"
}

function ensure_souffle_program_is_built_for_kafka() {
    local EXE="${1}"
    local FILE="${2}"
    local DIRNAME="$(dirname ${FILE})"
    local BASENAME="$(basename ${FILE})"
    local NAME="${BASENAME%%.*}"
    if [ ! -e "${DIRNAME}/${NAME}" ]
    then
        cd "${DIRNAME}"
        "${EXE}" \
            -ekafka \
            -D"${DIRNAME}" \
            -F"${DIRNAME}/facts" \
            -o"${DIRNAME}/${NAME}" \
            -r"${DIRNAME}/${NAME}.html" \
            "${FILE}"
        cd -
    fi
}

function ensure_kafka_depencencies_are_installed() {
    local CWD="${1}"
    ensure_librdkafka_is_installed "${CWD}"
    ensure_kafkacat_is_installed "${CWD}"
    ensure_apache_kafka_is_installed "${CWD}"
}

function ensure_souffle_test_case_is_built_for_kafka() {
    local SOUFFLE_ROOT="${1}"
    local TEST_CASE="${2}"
    local TESTSUITE_DIR="${SOUFFLE_ROOT}/tests/testsuite.dir"
    local TEST_CASE_FROM="${SOUFFLE_ROOT}/tests/${TEST_CASE}"
    local TEST_CASE_TO="${TESTSUITE_DIR}/${TEST_CASE}"
    ensure_file_is_copied "${TEST_CASE_FROM}" "${TEST_CASE_TO}"
    ensure_souffle_program_is_built_for_kafka "${SOUFFLE_ROOT}/src/souffle" "${TEST_CASE_TO}/$(basename "${TEST_CASE_TO}").dl"
}

function main() {

    [[ $(basename ${PWD}) != souffle ]] && exit 1

    rm -rf tests/testsuite.dir 

    local TEST_CASE="example/input_output_numbers"

    local KAFKA_HOST="localhost:9092"
    local KAFKA_DOCKER_PATH="${PWD}/kafka"
    export PATH="${PWD}/kafka/tmp/kafka_2.12-2.3.1/bin:${PATH}"

    ensure_kafka_depencencies_are_installed "${PWD}/kafka/tmp"
    ensure_jq_is_installed "${PWD}/kafka/tmp"
    ensure_souffle_is_built_for_kafka "${PWD}"
    ensure_souffle_test_case_is_built_for_kafka "${PWD}" "${TEST_CASE}"

    local EXE="${PWD}/tests/testsuite.dir/${TEST_CASE}/$(basename ${TEST_CASE})"

    ${EXE} -i-2 | jq

    local JSON_DATA=$(${EXE} -i-2)

    local RELATION_NAMES="$(echo ${JSON_DATA} | jq -r '.RelationNames | .[]')"
    local STRATUM_NAMES="$(echo ${JSON_DATA} | jq -r '.StratumNames | .[]')"

    ensure_docker_compose_is_up "${KAFKA_DOCKER_PATH}"
    ensure_kafka_topic_created ${KAFKA_HOST} _DEBUG_
    for_each "ensure_kafka_topic_created ${KAFKA_HOST}" default ${RELATION_NAMES}

    ${EXE} -i-1 &

    sleep 5s

    for_each_async "${EXE} -i" ${STRATUM_NAMES}

    wait

    read -p "Continue?"

    for_each "ensure_kafka_topic_deleted ${KAFKA_HOST}" default ${RELATION_NAMES}
    ensure_docker_compose_is_down "${KAFKA_DOCKER_PATH}"

    exit 0

}

main ${@:-}

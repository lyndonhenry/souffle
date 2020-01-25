#!/bin/bash

set -oue pipefail

function ensure_kafka_topic_created() {
    local KAFKA_HOST="${1}"
    local TOPIC="${2}"
    kafka-topics.sh --create --bootstrap-server "${KAFKA_HOST}" --topic "${TOPIC}" --replication-factor 1 --partitions 1  || :
}

function ensure_kafka_topic_deleted() {
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
  ##  sudo ldconfig
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
    if [ ! -e "${DIRNAME}/${BASENAME%.*}" ]
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

function ensure_souffle_program_is_built() {
    local EXE="${1}"
    local FILE="${2}"
    local ARGS="${3}"
    local DIRNAME="$(dirname ${FILE})"
    local BASENAME="$(basename ${FILE})"
    local NAME="${BASENAME%%.*}"

    if [ ! -e "${DIRNAME}/${NAME}" ]
    then
        cd "${DIRNAME}"
        "${EXE}" \
            -D"${DIRNAME}" \
            -F"${DIRNAME}/facts" \
            -o"${DIRNAME}/${NAME}" \
            -r"${DIRNAME}/${NAME}.html" \
            ${ARGS} \
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

function ensure_souffle_test_case_is_built() {
    local SOUFFLE_ROOT="${1}"
    local TEST_CASE="${2}"
    local SOUFFLE_ARGS="${3:-}"
    local TESTSUITE_DIR="${SOUFFLE_ROOT}/tests/testsuite.dir"
    local TEST_CASE_FROM="${SOUFFLE_ROOT}/tests/${TEST_CASE}"
    local TEST_CASE_TO="${TESTSUITE_DIR}/${TEST_CASE}"
    ensure_file_is_copied "${TEST_CASE_FROM}" "${TEST_CASE_TO}"
    ensure_souffle_program_is_built "${SOUFFLE_ROOT}/src/souffle" "${TEST_CASE_TO}/$(basename "${TEST_CASE_TO}").dl" "${SOUFFLE_ARGS}"
}

function ensure_test_case_passes() {
    local TEST_CASE="${1}"
    local SOUFFLE_ARGS="${2:-}"
    local EXE_ARGS="${3:-}"
    local TESTSUITE_DIR="${PWD}/tests/testsuite.dir"
    local TEST_CASE_ROOT="${TESTSUITE_DIR}/${TEST_CASE}"
    local EXE="${TEST_CASE_ROOT}/$(basename ${TEST_CASE})"
    # remove the testsuite directotry
    rm -rf "${TESTSUITE_DIR}"
    # ensute that test case is built
    ensure_souffle_test_case_is_built "${PWD}" "${TEST_CASE}" "${SOUFFLE_ARGS}"
    # show the line count of the expected output files
    wc -l "${TEST_CASE_ROOT}"/*.csv > "${TEST_CASE_ROOT}"/expected.txt 
    # remove the expected output files, these are overridden by actual outputs on execution
    rm -rf "${TEST_CASE_ROOT}"/*.csv
    # run the program
    ${EXE} "${3:-}"
    # show the line count of the actual output files, the user should compare this to the expected produced above
    wc -l "${TEST_CASE_ROOT}"/*.csv > "${TEST_CASE_ROOT}"/actual.txt 
    # diff the actual vs expected output
    if [ "$(diff "${TEST_CASE_ROOT}"/actual.txt "${TEST_CASE_ROOT}"/expected.txt)" ]
    then
        echo
        echo "ERROR"
        echo " TEST_CASE='${TEST_CASE}'"
        echo " SOUFFLE_ARGS='${SOUFFLE_ARGS}'"
        echo " EXE_ARGS='${EXE_ARGS}'"
        echo " ACTUAL='"
        cat "${TEST_CASE_ROOT}"/actual.txt
        echo " '"
        echo " EXPECTED='"
        cat "${TEST_CASE_ROOT}"/expected.txt
        echo " '"
        echo " DIFF='"
        diff "${TEST_CASE_ROOT}"/actual.txt "${TEST_CASE_ROOT}"/expected.txt
        echo " '"
        echo
        read -p "Continue?"
    else
        echo
        echo "SUCCESS"
        echo " TEST_CASE='${TEST_CASE}'"
        echo " SOUFFLE_ARGS='${SOUFFLE_ARGS}'"
        echo " EXE_ARGS='${EXE_ARGS}'"
        echo " ACTUAL='"
        cat "${TEST_CASE_ROOT}"/actual.txt
        echo " '"
        echo " EXPECTED='"
        cat "${TEST_CASE_ROOT}"/expected.txt
        echo " '"
        echo " DIFF='"
        diff "${TEST_CASE_ROOT}"/actual.txt "${TEST_CASE_ROOT}"/expected.txt
        echo " '"
        echo
    fi
}

function ensure_kafka_test_case_passes() {
    local KAFKA_HOST="${1}"
    local TEST_CASE="${2}"
    local SOUFFLE_ARGS="${3:-}"
    local EXE_ARGS="${4:-}"
    local TESTSUITE_DIR="${PWD}/tests/testsuite.dir"
    local TEST_CASE_ROOT="${TESTSUITE_DIR}/${TEST_CASE}"
    local EXE="${TEST_CASE_ROOT}/$(basename ${TEST_CASE})"
    # remove the testsuite directotry
    rm -rf "${TESTSUITE_DIR}"
    # ensute that test case is built
    ensure_souffle_test_case_is_built "${PWD}" "${TEST_CASE}" "${SOUFFLE_ARGS}"
    # show the line count of the expected output files
    wc -l "${TEST_CASE_ROOT}"/*.csv > "${TEST_CASE_ROOT}"/expected.txt 
    # remove the expected output files, these are overridden by actual outputs on execution
    rm -rf "${TEST_CASE_ROOT}"/*.csv
    # extract stratum names and relation names from json metadata
    local JSON_DATA="$(${PWD}/src/souffle ${EXE}.dl --show=topsort-info)"
    local RELATION_NAMES="$(echo ${JSON_DATA} | jq -r '.RelationNames | .[]')" 
    local STRATUM_NAMES="$(echo ${JSON_DATA} | jq -r '.StratumNames | .[]')"
    # ensure that all program specific topics are initially empty by deleting them
    for_each_async "ensure_kafka_topic_deleted ${KAFKA_HOST}" ${RELATION_NAMES}
    wait
    sleep 1s
    # ensure that all program specific topics exist
    for_each_async "ensure_kafka_topic_created ${KAFKA_HOST}" ${RELATION_NAMES}
    wait
    # run all program strata as subprograms
    for_each_async "${EXE} -Xmetadata.broker.list=${KAFKA_HOST} -i" -1 ${STRATUM_NAMES}
    wait
    # show the line count of the actual output files, the user should compare this to the expected produced above
    wc -l "${TEST_CASE_ROOT}"/*.csv > "${TEST_CASE_ROOT}"/actual.txt 
    # diff the actual vs expected output
    if [ "$(diff "${TEST_CASE_ROOT}"/actual.txt "${TEST_CASE_ROOT}"/expected.txt)" ]
    then
        echo
        echo "ERROR"
        echo " TEST_CASE='${TEST_CASE}'"
        echo " SOUFFLE_ARGS='${SOUFFLE_ARGS}'"
        echo " EXE_ARGS='${EXE_ARGS}'"
        echo " ACTUAL='"
        cat "${TEST_CASE_ROOT}"/actual.txt
        echo " '"
        echo " EXPECTED='"
        cat "${TEST_CASE_ROOT}"/expected.txt
        echo " '"
        echo " DIFF='"
        diff "${TEST_CASE_ROOT}"/actual.txt "${TEST_CASE_ROOT}"/expected.txt
        echo " '"
        echo
        read -p "Continue?"
    else
        echo
        echo "SUCCESS"
        echo " TEST_CASE='${TEST_CASE}'"
        echo " SOUFFLE_ARGS='${SOUFFLE_ARGS}'"
        echo " EXE_ARGS='${EXE_ARGS}'"
        echo " ACTUAL='"
        cat "${TEST_CASE_ROOT}"/actual.txt
        echo " '"
        echo " EXPECTED='"
        cat "${TEST_CASE_ROOT}"/expected.txt
        echo " '"
        echo " DIFF='"
        diff "${TEST_CASE_ROOT}"/actual.txt "${TEST_CASE_ROOT}"/expected.txt
        echo " '"
        echo
    fi
}

function main() {

    # ensure that we are in the root directory of the souffle project
    [[ $(basename ${PWD}) != souffle ]] && exit 1

    # ensure that the testsuite directory does does not exist yet
    local TESTSUITE_DIR="${PWD}/tests/testsuite.dir"

    # @@@TODO: try with 'input_output_numbers_recursive'

    # set the test case used by this script
    local TEST_CASE="example/input_output_numbers"

    # set variables for kafka
    local KAFKA_HOST="localhost:9092"
    local KAFKA_DOCKER_PATH="${PWD}/kafka"
    export PATH="${PWD}/kafka/tmp/kafka_2.12-2.3.1/bin:${PATH}"

    # ensure dependencies are installed
    ensure_kafka_depencencies_are_installed "${PWD}/kafka/tmp"
    ensure_jq_is_installed "${PWD}/kafka/tmp"

    # ensure that souffle is built for kafka
    ensure_souffle_is_built_for_kafka "${PWD}"

    # start the kafka broker
    ensure_docker_compose_is_up "${KAFKA_DOCKER_PATH}"

    # ensure that the debugging topic exists
    ensure_kafka_topic_created ${KAFKA_HOST} _DEBUG_

    # set the executable for the test case
    local EXE="${TESTSUITE_DIR}/${TEST_CASE}/$(basename ${TEST_CASE})"

    # run tests for no -e
    ensure_test_case_passes "${TEST_CASE}"
    ensure_test_case_passes "${TEST_CASE}" --custom=use-general
    ensure_test_case_passes "${TEST_CASE}" --custom=use-general,use-general-producers

    # run tests for -efile
    ensure_test_case_passes "${TEST_CASE}" "-efile"
    ensure_test_case_passes "${TEST_CASE}" "-efile --custom=use-general"
    ensure_test_case_passes "${TEST_CASE}" "-efile --custom=use-general,use-general-producers"

    # run tests for -ekafka, tested and working
    ensure_kafka_test_case_passes "${KAFKA_HOST}" "${TEST_CASE}" "-ekafka" 
    ensure_kafka_test_case_passes "${KAFKA_HOST}" "${TEST_CASE}" "-ekafka --custom=use-general"
    ensure_kafka_test_case_passes "${KAFKA_HOST}" "${TEST_CASE}" "-ekafka --custom=use-general,use-general-producers"
    ensure_kafka_test_case_passes "${KAFKA_HOST}" "${TEST_CASE}" "-ekafka --custom=use-general,use-general-producers,use-general-consumers"

    # prompt the user to continue with cleanup
    read -p "Continue?"

    # stop the kafka broker
    ensure_docker_compose_is_down "${KAFKA_DOCKER_PATH}"

    exit 0

}

main ${@:-}

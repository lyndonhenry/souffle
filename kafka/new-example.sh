#!/bin/bash

set -ouex pipefail

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
        (echo "Begin '${f} ${x}'" && \
        ${f} ${x} && \
        echo "End '${f} ${x}'") &
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
    ensure_directory_exists "${CWD}"
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
    if [ ! -e "/usr/local/lib/librdkafka.a" ]
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
    local KAFKA_PATH="${2}"
    local APACHE_KAFKA_URL="http://mirror.vorboss.net/apache/kafka/2.3.1/kafka_2.12-2.3.1.tgz"
    if [ ! -e "${KAFKA_PATH}" ]
    then
        ensure_tarball_is_downloaded_and_unzipped "${CWD}" "${APACHE_KAFKA_URL}"
        ensure_file_is_moved "${CWD}/kafka_2.12-2.3.1" "${KAFKA_PATH}"
    fi
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
    ensure_autoconf_project_is_built "${CWD}" "--enable-kafka --enable-debug --disable-libz --disable-sqlite"
}

function ensure_souffle_program_is_built() {
    local EXE="${1}"
    local FILE="${2}"
    local ARGS="${3}"
    local DIRNAME="$(dirname ${FILE})"
    local BASENAME="$(basename ${FILE})"
    local NAME="${BASENAME%%.*}"
    local COMMAND=""
    COMMAND+="${EXE}"
    COMMAND+=" -D${DIRNAME} "
    COMMAND+=" -F${DIRNAME}/facts "
    COMMAND+=" -o${DIRNAME}/${NAME} "
    COMMAND+=" -r${DIRNAME}/${NAME}.html "
    COMMAND+=" ${ARGS} "
    COMMAND+=" ${FILE} "
    if [ ! -e "${DIRNAME}/${NAME}" ]
    then
        cd "${DIRNAME}"
        # show transformed ram
        ${COMMAND} --show=transformed-ram
        # show global config
        ${COMMAND} --show=global-config
        # echo the command
        echo
        echo "${COMMAND}"
        echo
        # run souffle
        ${COMMAND}
        cd -
    fi

}

function ensure_kafka_depencencies_are_installed() {
    local CWD="${1}"
    local KAFKA_PATH="${2}"
    ensure_librdkafka_is_installed "${CWD}"
    ensure_kafkacat_is_installed "${CWD}"
    ensure_apache_kafka_is_installed "${CWD}" "${KAFKA_PATH}"
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
    ${EXE} ${EXE_ARGS}
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
    echo "Deleting topics if they exist..."
    local EXE_EXTRA_ARGS=""
    EXE_EXTRA_ARGS+=" -Xmetadata.broker.list=${KAFKA_HOST} "
    EXE_EXTRA_ARGS+=" -Xcustom.create-topics=false "
    EXE_EXTRA_ARGS+=" -Xcustom.run-program=false "
    EXE_EXTRA_ARGS+=" -Xcustom.delete-topics=true "
    EXE_EXTRA_ARGS+=" -Xcustom.disable-stdout=false "
    EXE_EXTRA_ARGS+=" -Xcustom.disable-stderr=false "
    EXE_EXTRA_ARGS+=" -Xcustom.unique-id=XXX "
    ${EXE} ${EXE_ARGS} ${EXE_EXTRA_ARGS}
    sleep 1s
    # ensure that all program specific topics exist
    echo "Creating topics..."
    local EXE_EXTRA_ARGS=""
    EXE_EXTRA_ARGS+=" -Xmetadata.broker.list=${KAFKA_HOST} "
    EXE_EXTRA_ARGS+=" -Xcustom.create-topics=true "
    EXE_EXTRA_ARGS+=" -Xcustom.run-program=false "
    EXE_EXTRA_ARGS+=" -Xcustom.delete-topics=false "
    EXE_EXTRA_ARGS+=" -Xcustom.disable-stdout=false "
    EXE_EXTRA_ARGS+=" -Xcustom.disable-stderr=false "
    EXE_EXTRA_ARGS+=" -Xcustom.unique-id=XXX "
    ${EXE} ${EXE_ARGS} ${EXE_EXTRA_ARGS}
    # run all program strata as subprograms
    echo "Running program..."
    local EXE_EXTRA_ARGS=""
    EXE_EXTRA_ARGS+=" -Xmetadata.broker.list=${KAFKA_HOST} "
    EXE_EXTRA_ARGS+=" -Xcustom.create-topics=false "
    EXE_EXTRA_ARGS+=" -Xcustom.run-program=true "
    EXE_EXTRA_ARGS+=" -Xcustom.delete-topics=false "
    EXE_EXTRA_ARGS+=" -Xcustom.unique-id=XXX "
    for_each_async "${EXE} ${EXE_ARGS} ${EXE_EXTRA_ARGS} -i" -2 -3 ${STRATUM_NAMES}
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

function ensure_testsuite_passes() {
    # @TODO (lh): get testing right here
    local JOBS=$(nproc || sysctl -n hw.ncpu || echo 2)
    local SOUFFLE_CATEGORY="FastEvaluation"
    local SC=""
    SC+=" -j${JOBS} "
    # @TODO (lh): test with and without this
    #SC+=" -Xuse-immutable-global-config "
    local SOUFFLE_CONFS=""
    SOUFFLE_CONFS+="${SC}"
   #SOUFFLE_CONFS+=",${SC} -c"
   #SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-file"
   #SOUFFLE_CONFS+=",${SC} -Xuse-general"
   #SOUFFLE_CONFS+=",${SC} -c -Xuse-general"
   #SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-file -Xuse-general"
   #SOUFFLE_CONFS+=",${SC} -Xuse-general -Xuse-general-producers"
   #SOUFFLE_CONFS+=",${SC} -c -Xuse-general -Xuse-general-producers"
   #SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-file -Xuse-general -Xuse-general-producers"
   #SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-kafka"
   #SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-kafka -Xuse-general"
   #SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-kafka -Xuse-general -Xuse-general-producers"
    SOUFFLE_CONFS+=",${SC} -c -Xuse-engine-kafka -Xuse-general -Xuse-general-producers -Xuse-general-consumers"
    export SOUFFLE_CATEGORY="${SOUFFLE_CATEGORY}"
    export SOUFFLE_CONFS="${SOUFFLE_CONFS}"
    make clean || :
    ./bootstrap
    SOUFFLE_CATEGORY=${SOUFFLE_CATEGORY} SOUFFLE_CONFS=${SOUFFLE_CONFS} ./configure --enable-kafka --enable-debug --disable-libz --disable-sqlite
    make -j${JOBS}
    TESTSUITEFLAGS="-j${JOBS}" make check -j${JOBS}
}

function ensure_sudo_permissions() {
    sudo echo
}

function main() {

    # ensure we have sudo permissions
    ensure_sudo_permissions

    # ensure that we are in the root directory of the souffle project
    [[ $(basename ${PWD}) != souffle ]] && exit 1

    # ensure that the testsuite directory does does not exist yet
    local TESTSUITE_DIR="${PWD}/tests/testsuite.dir"

    # set the test case used by this script
    # @TODO (lh): local TEST_CASE="example/input_output_numbers"
    local TEST_CASE="evaluation/neg6"

    # make a temp directory for dependencies
    local TMP_PATH="/tmp/souffle"

    # use a home directory for kafka
    local KAFKA_PATH="${HOME}/.kafka"

    # set the path to use the kafka scripts
    export PATH="${KAFKA_PATH}/bin:${PATH}"

    # ensure dependencies are installed
    ensure_kafka_depencencies_are_installed "${TMP_PATH}" "${KAFKA_PATH}"
    ensure_jq_is_installed "${TMP_PATH}"

    # ensure that souffle is built for kafka
    ensure_souffle_is_built_for_kafka "${PWD}"

    # run tests for no -e

    # normal seminaive evaluation
    ensure_test_case_passes "${TEST_CASE}"
    # generalized seminaive evaluation
    ensure_test_case_passes "${TEST_CASE}" -Xuse-general
    # generalized seminaive evaluation with output relations written to files inside fixpoint loop
    ensure_test_case_passes "${TEST_CASE}" -Xuse-general -Xuse-general-producers

    # run tests for -Xuse-engine-file

    # normal seminaive evaluation with intermediate results written to and read from files
    ensure_test_case_passes "${TEST_CASE}" "-Xuse-engine-file"
    # generalized seminaive evaluation with intermediate results written to and read from files outside of fixpoint loop
    ensure_test_case_passes "${TEST_CASE}" "-Xuse-engine-file -Xuse-general"
    # generalized seminaive evaluation with intermediate results read from files outside of fixpoint loop and written to files inside fixpoint loop
    ensure_test_case_passes "${TEST_CASE}" "-Xuse-engine-file -Xuse-general -Xuse-general-producers"

    # run tests for -Xuse-engine-kafka

    # set variables for kafka
    local KAFKA_HOST="localhost:9092"
    local KAFKA_DOCKER_PATH="${PWD}/kafka"

    # start the kafka broker
    ensure_docker_compose_is_down "${KAFKA_DOCKER_PATH}"
    ensure_docker_compose_is_up "${KAFKA_DOCKER_PATH}"

    # normal seminaive evaluation with intermediate results produced to and consumed from kafka topics
    ensure_kafka_test_case_passes "${KAFKA_HOST}" "${TEST_CASE}" "-Xuse-engine-kafka"
    # generalized seminaive evaluation with intermediate results produced to and consumed from kafka topics outside of fixpoint loop
    ensure_kafka_test_case_passes "${KAFKA_HOST}" "${TEST_CASE}" "-Xuse-engine-kafka -Xuse-general"
    # generalized seminaive evaluation with intermediate results consumed from kafka topics outside of fixpoint loop and produced to kafka topics inside of fixpoint loop
    ensure_kafka_test_case_passes "${KAFKA_HOST}" "${TEST_CASE}" "-Xuse-engine-kafka -Xuse-general -Xuse-general-producers"
    # generalized seminaive evaluation with intermediate results produced to and consumed from kafka topics inside of fixpoint loop
    ensure_kafka_test_case_passes "${KAFKA_HOST}" "${TEST_CASE}" "-Xuse-engine-kafka -Xuse-general -Xuse-general-producers -Xuse-general-consumers"

    # run the testsuite
    ensure_testsuite_passes

    # run the kafka testsuite
    ensure_kafka_testsuite_passes

    # stop the kafka broker
    ensure_docker_compose_is_down "${KAFKA_DOCKER_PATH}"

    exit 0

}

#ensure_docker_compose_is_down "${PWD}/kafka"
#ensure_docker_compose_is_up "${PWD}/kafka"
#export PATH="${HOME}/.kafka/bin:${PATH}"
#make -j8 
# @TODO: this appears to work, but the file contains only blank lines, the problem is that symbols do not seem to be resolved from the table
#ensure_kafka_test_case_passes "localhost:9092" "evaluation/independent_body1" "-Xuse-engine-kafka -Xuse-general -Xuse-general-producers -Xuse-general-consumers"
#cd tests 
#./testsuite 38 42 44 50 56 72 78
#exit
#ensure_testsuite_passes

main ${@:-}

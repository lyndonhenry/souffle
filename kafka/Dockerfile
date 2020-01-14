#
#   First phase of the build - Build Souffle itself
#   
FROM ubuntu:18.04

RUN apt-get update && apt-get install -y \
        autoconf \
        automake \
        bison \
        build-essential \
        doxygen \
        flex \
        g++ \
        git \
        graphviz \
        libffi-dev \
        libsqlite3-dev \
        libtool \
        lsb-release \
        libncurses5-dev \
        make \
        mcpp \
        python \
        sqlite \
        zlib1g-dev \
        librdkafka-dev

ARG SOUFFLE_CC="gcc"
ARG SOUFFLE_CXX="g++"
ARG SOUFFLE_CONFIGURE_OPTIONS="--enable-kafka"
ARG SOUFFLE_GITHUB_USER="kjezek"
ARG SOUFFLE_GIT_BRANCH="master"
ARG SOUFFLE_GIT_REVISION="HEAD"
ARG SOUFFLE_MAKE_JOBS="8"

ENV CC "${SOUFFLE_CC}"
ENV CXX "${SOUFFLE_CXX}"

WORKDIR /tmp

RUN git clone https://github.com/${SOUFFLE_GITHUB_USER}/souffle souffle
WORKDIR /tmp/souffle
RUN git pull && git checkout ${SOUFFLE_GIT_BRANCH} && git reset --hard ${SOUFFLE_GIT_REVISION} && git clean -xdf

RUN ./bootstrap 
RUN ./configure ${SOUFFLE_CONFIGURE_OPTIONS}
RUN make -j${SOUFFLE_MAKE_JOBS}

#
#   Second phase of the build - build a datalog program
#
FROM ubuntu:18.04

COPY --from=0 /tmp/souffle/ souffle/

# Install libraries for souffle to run
RUN apt-get update && apt-get install -y \
        autoconf \
        automake \
        bison \
        build-essential \
        doxygen \
        flex \
        g++ \
        git \
        graphviz \
        libffi-dev \
        libsqlite3-dev \
        libtool \
        lsb-release \
        libncurses5-dev \
        make \
        mcpp \
        python \
        sqlite \
        zlib1g-dev \
        wget \
        tar \
        default-jre \
        curl \
        jq \
        docker.io \
        kafkacat \
        bc \
        docker-compose

ARG PROGRAM
ADD ${PROGRAM} .

# Install Kafka for CLI usage
RUN wget https://www.apache.org/dist/kafka/2.3.1/kafka_2.11-2.3.1.tgz && tar xf kafka_2.11-2.3.1.tgz && ln --symbolic kafka_2.11-2.3.1 kafka && chmod +x /kafka/bin/
ENV PATH="/kafka/bin/:${PATH}"

RUN mkdir output && mkdir facts

# Compile a program by Souffle
# The program is passed in as a build argument (--build-argument) 
RUN ./souffle/src/souffle \
            -efile \
            -Doutput \
            -Ffacts \
            -oprogram \
            -rprogram.html \
            ${PROGRAM}

ADD cli/kafka_api.sh .
ADD cli/execute_strata.sh .
ADD cli/docker-compose.yml .
ADD cli/execute_master.sh .
ADD cli/execute_standalone.sh .

ADD native/execute_master_native.sh .

RUN chmod +x *.sh
RUN cp *.json program.json

CMD ./execute_master.sh program

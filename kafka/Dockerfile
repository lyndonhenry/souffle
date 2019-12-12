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
        zlib1g-dev

ARG SOUFFLE_CC="gcc"
ARG SOUFFLE_CXX="g++"
ARG SOUFFLE_CONFIGURE_OPTIONS=""
ARG SOUFFLE_GITHUB_USER="kjezek"
ARG SOUFFLE_GIT_BRANCH="feature/1-messaging"
ARG SOUFFLE_GIT_REVISION="HEAD"
ARG SOUFFLE_MAKE_JOBS="2"

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
        jq

ARG PROGRAM
ADD ${PROGRAM} .

RUN mkdir output && mkdir facts

# Compile a program by Souffle
# The program is passed in as a build argument (--build-argument) 
# RUN ./souffle/src/souffle \
#             -efile \
#             -Doutput \
#             -Ffacts \
#             -oprogram \
#             ${PROGRAM}

CMD /bin/bash

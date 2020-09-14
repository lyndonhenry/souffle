FROM ubuntu

ENV DEBIAN_FRONTEND="noninteractive"
ENV TZ="America/New_York"

RUN apt-get update -y && apt-get upgrade -y && apt-get install -y git sudo tzdata

ARG DOCKER_USER

RUN useradd --create-home --shell /bin/bash ${DOCKER_USER} && \
      echo "${DOCKER_USER}:password" | chpasswd && \
      adduser ${DOCKER_USER} sudo && \
      sed -i 's/^%sudo.*$/%sudo ALL=(ALL:ALL) NOPASSWD:ALL/' /etc/sudoers

USER ${DOCKER_USER}
WORKDIR /home/${DOCKER_USER}
ENV USER="${DOCKER_USER}"

ARG GITHUB_BRANCH
ARG GITHUB_USER

RUN git clone https://github.com/${GITHUB_USER}/souffle && \
    cd souffle && \
    git checkout ${GITHUB_BRANCH} && \
    git pull origin ${GITHUB_BRANCH} && \
    git clean -xdf

WORKDIR /home/${DOCKER_USER}/souffle

# adds kafka scripts and souffle-kafka.sh to path
ENV PATH="/home/${DOCKER_USER}/.kafka/bin:/home/${DOCKER_USER}/souffle/kafka:${PATH}"

RUN ./kafka/souffle-kafka.sh --build

# install the aws cli
WORKDIR /tmp

RUN sudo apt-get install -y zip
RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
RUN unzip awscliv2.zip
RUN sudo ./aws/install

WORKDIR /home/${DOCKER_USER}/souffle

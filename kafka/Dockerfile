FROM ubuntu

ENV DEBIAN_FRONTEND="noninteractive"
ENV TZ="America/New_York"

WORKDIR /tmp

RUN apt-get update -y && apt-get upgrade -y && apt-get install -y git sudo curl tmux tzdata zip vim

RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
RUN unzip awscliv2.zip
RUN ./aws/install

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

# adds rust, kafka scripts, and souffle-kafka.sh to path
ENV PATH="/home/${DOCKER_USER}/.cargo/bin:/home/${DOCKER_USER}/.kafka/bin:/home/${DOCKER_USER}/souffle/kafka:${PATH}"

RUN ./kafka/docker.sh --build

WORKDIR /home/${DOCKER_USER}/souffle

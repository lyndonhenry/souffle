FROM ubuntu

RUN apt-get update -y && \
      apt-get upgrade -y && \
      apt-get dist-upgrade -y && \
      apt-get install -y \
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
      zlib1g-dev && \
      apt-get autoremove -y && \
      apt-get autoclean -y && \
      apt-get clean -y

RUN apt-get install -y \
      curl \
      docker-compose \
      jq \
      sudo

ARG DOCKER_USER="nobody"

RUN useradd --create-home --shell /bin/bash ${DOCKER_USER} && \
      echo "${DOCKER_USER}:password" | chpasswd && \
      adduser ${DOCKER_USER} sudo

USER ${DOCKER_USER}

RUN mkdir /home/${DOCKER_USER}/souffle

VOLUME /home/${DOCKER_USER}/souffle

WORKDIR /home/${DOCKER_USER}/souffle

ENTRYPOINT ["/bin/bash"]

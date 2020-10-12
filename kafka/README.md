# README

Welcome to Souffle on Kafka.

To get started, run 

~~~
./kafka/souffle-on-kafka-docker --run /bin/bash
~~~

The experiments can be run from 
within the docker container with

~~~
./kafka/souffle-on-kafka-experiments.sh
~~~

To create a new system from scratch, do:

~~~
./kafka/souffle-on-kafka-docker.sh --clean &&
./kafka/souffle-on-kafka-docker.sh --push &&
./kafka/souffle-on-kafka-docker.sh --run /bin/bash
~~~

then, in the Docker container:

~~~
./kafka/souffle-on-kafka-experiments.sh --reset
./kafka/souffle-on-kafka-experiments.sh
~~~

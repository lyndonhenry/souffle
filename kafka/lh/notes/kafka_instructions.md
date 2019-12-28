# Kamil's Kafka Instructions

## Kamil's Email

just run it, typing following in the directory where docker-compose.yml is:

~~~
docker-compose up -d
~~~

also download Apache Kafka (http://kafka.apache.org) in parallel as it has all the command-line tools. You can then run for instance:

~~~
bin/kafka-topics.sh --create --bootstrap-server localhost:9092 --replication-factor 1 --partitions 1  --topic test
 bin/kafka-console-producer.sh --broker-list localhost:9092 --topic test
 bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test
~~~

- when you run the producer type a few messages from the keyboard. These messages will be then shown by running the consumer.

For debugging it is good to install kafkacat, i allows for instance to run:

~~~
kafkacat -b localhost:9092 -L
Metadata for all topics (from broker 1001: localhost:9092/1001):
 1 brokers:
  broker 1001 at localhost:9092 (controller)
 2 topics:
  topic "test" with 1 partitions:
    partition 0, leader 1001, replicas: 1001, isrs: 1001
~~~


Hope it helps. Feel free to ask questions.

## Lyndon's Notes

When you are done, do

~~~
docker-compose down
~~~

# README

Welcome to Souffle on Kafka.

## Running an Example

$ ./kafka/souffle-on-kafka-docker.sh
$ ./kafka/script-to-generate-datasets.sh
> Generated example datasets, press enter to continue.
> Ctrl-C
```
$ ./kafka/script-to-generate-experiments.sh
> Generated first example experiment, press enter to continue.
> Generated all example experiments, press enter to continue.
> Ctrl-C
```
~~~
$ ./kafka/script-to-run-experiments.sh
> Ran example experiments, press enter to continue.
> Ctrl-C
~~~

Note that 
```
$ ./kafka/souffle-on-kafka-docker.sh --clean
```
can be used to delete all Docker containers.

Also the environment variables in 

```
./kafka/.env
```
must use the current branch and be up to date.

- do notes on how to generate everything up to an example and cancel with read -p
- then test on both log files and without them for one example

If you can't run `docker` without sudo, do
```
$ sudo groupadd docker
$ sudo usermod -aG docker $USER
$ su -s $USER
$ docker run hello-world
```

If you aren't running a `docker swarm` before the call to `script-to-run-experiments.sh`, do

```
docker swarm init
```

## Files of Interest

This briefly explains the role of certain files in the system. 
Please look at the scripts themselves to understand the details of the implementation.
The main script that shows how to go from a Datalog program and some data for it is `./kafka/souffle-on-kafka.sh`.
Many of these scripts will need to be modified, as they are hardcoded to a particular S3 bucket.

### Scripts

- `./kafka/rust`: Has rust code for generating datasets and benchmarks.
- `./kafka/log-parser`: Has java code for analysing log files for results analysis.
- `./kafka/Dockerfile`: Dockerfile for the Souffle on Kafka system.
- `./kafka/docker.sh`: Script that runs is included in every Souffle on Kafka docker build. Used by `./kafka/Dockerfile`.
- `./kafka/souffle-on-kafka.sh`: Main script used to generate a Souffle on Kafka deployment from a given config.
- `./kafka/souffle-on-kafka-docker.sh`: Commands for building and running Souffle on Kafka in a Docker container.

### C++ Code

- `./src/Kafka.h`: Contains all Kafka-related code.
- `./src/WriteStreamKafka.h`: Write stream for Kafka. Contains logic on message batching at 100KB message size, and polling.
- `./src/ReadStreamKafka.h`: Read stream for Kafka.
- `./src/AstTranslator.cpp`: Implements streaming seminaive, see RAM program generated using debug report.
- `./src/AstSemanticChecker.cpp`: Disables streaming seminaive if aggregation is present.


## Configuration

A brief explanation is given of each of the configuration parameters that defines a Souffle on Kafka execution over a particular program and input.
Experiments are obtained by use of different combinations of configuration parameters on different programs and datasets. 

- *Engine*: The engine determines what happens to intermediate relations during program execution. Any relation is considered intermediate if it occurs in the body of a rule of a stratum other than its own.
    - *Normal*: Intermediate relations are produced to and consumed from memory.
    - *Kafka*: Intermediate relations are produced to and consumed from Kafka topics.
- *Machines*: This states whether strata are executed on one machine/docker instance, or multiple machines/docker instances.
    - *One*: The program is executed using one machine/docker instance.
    - *Many*: The program is executed as many machines/docker instances. 
- *Processes*:  This states whether strata may be executed as multiple concurrent processes, or the program exected as a single process.
    - *One*: The program may be executed as a single process on one machine, evaluating strata in topological order of their SCC graph.
    - *Many*: The program may be executed as multiple concurrent processes.
- *Threads*: This states whether executions may use one or many threads. Use of multithreading is valid under all configurations.
    - *One*: The execution may run with one thread.
    - *Many*:  The execution may run with multiple threads.
- *Algorithm*: This states the evaluation algorithm used for the program execution.
    - *SNE*:   Standard seminaive evaluation.
    - *GSNE*: Generalised SNE, where each non-negated EDB relation is evaluated in the manner of an IDB relation.
    - *GPSNE*: GSNE, but with production of new IDB tuples to successor strata occurring at the end of each epoch.
    - *GPCSNE*: GPSNE, but with consumption of new EDB tuples from predecessor strata occuring at the beginning of each epoch. Note that this and the SNE algorithm are the ones that are useful, the others are intermediate algorithms that may be used to measure particular overheads.

We follow with a list of each of the options that must be passed at the command line in the call to Souffle to obtain the desired configuration.
Each of these are appended to a regular invocation of Souffle to enable the desired behaviour (e.g. `./src/souffle -Ffacts -Dcsvs myprogram.dl`).
Use of some options is dependent upon deployment scripts, but may require particular arguments to Souffle.
These are passed to Souffle by the `./kafka/souffle-on-kafka.sh` script.

- Engine
    - Normal: enabled by default
    - File: `-Xuse-engine-file`
    - Kafka: `-Xuse-engine-kafka`
- Machine
    - One: (determined by deployment scripts)
    - Many: (determined by deployment scripts, only allowed if `-Xuse-engine-kafka`)
- Processes
    - One: (determined by deployment scripts)
    - Many: (determined by deployment scripts, only allowed if `-Xuse-engine-kafka`)
- Threads
    - One: `-j1`
    - Many: `-j8` note that this will run with 8 threads.
- Algorithm
    - SNE: (by default)
    - GSNE: `-Xuse-general`
    - GPSNE: `-Xuse-general -Xuse-general-producers`
    - GPCSNE: `-Xuse-general -Xuse-general-producers -Xuse-general-consumers`

A list of examples follows. 
Here we assume that the Souffle executable at `./src/souffle` is on your `$PATH`, that "facts" is a directory with `.facts` files, "csv" is an output directory, and "prog.dl" is a Datalog program.

- Compiler mode, eight threads. 
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c`
- General strategy, compiler mode, eight threads.
    -  `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-general`
- General producers strategy, compiler mode, eight threads.
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-general -Xuse-general-producers`
- File engine, compiler mode, eight threads.
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-engine-file`
- General strategy, file engine, compiler mode, eight threads.
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-engine-file -Xuse-general`
- General producers strategy, file engine, compiler mode, eight threads.
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-engine-file -Xuse-general -Xuse-general-producers`
- Kafka engine, compiler mode, eight threads.
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-engine-kafka`
- General strategy, kafka engine, compiler mode, eight threads.
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-engine-kafka -Xuse-general`
- General producers strategy, kafka engine, compiler mode, eight threads.
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-engine-kafka -Xuse-general -Xuse-general-producers`
- General consumers strategy, kafka engine, compiler mode, eight threads.
    - `souffle -Ffacts -Dcsv prog.dl -j8 -c -Xuse-engine-kafka -Xuse-general -Xuse-general-producers -Xuse-general-consumers`

## Experiments

The experiments are managed by bash scripts.

The scripts do not depend on each other, and no other script depends on them -- they are merely helper scripts.

The scripts are:

1. `script-to-run-in-docker.sh` 
2. `script-to-generate-datasets.sh` 
3. `script-to-generate-experiments.sh`
4. `script-to-run-experiments.sh`
5. `script-to-analyse-logs.sh`
6. `script-to-generate-results.sh`

Each script is to be run from the Souffle root directory, i.e. `./kafka/my-script.sh` and not `cd kafka && ./my-script.sh`.

The scripts correspond to each of the steps involved in running the experiments, and are presented in order.

The purpose of each script is as follows:

- `script-to-run-in-docker.sh`
    - Pulls and runs the Docker image for Souffle on Kafka.
    - The script shows which parameters to pass `docker run`, to mount directories.
    - All other scripts should be run from within the Docker container that this script pulls and runs.
    
- `script-to-generate-datasets.sh`
    - Downloads all datasets for the experiments, formats them, and places them in the S3 bucket.
    - This uses the code in `./kafka/rust/souffle_on_kafka.sh`.

- `script-to-generate-experiments.sh`
    - Generates all `docker-compose.yml` files for the experiments.
    - Each Docker compose file is placed in a directory that uniquely identifies the experiment it runs.
    - These are placed a subdirectory of `s3://souffle-on-kafka/docker-compose/`.
    - This subdirectory is either `example`, `yes-cloud`, or `no-cloud`.
    - The `example` docker compose files are used to test the system.
    - The `yes-cloud` docker compose files **are to be run in Azure**.
    - The `no-cloud` docker compose files **are to be run on plang8**.
    - This uses the code in `./kafka/rust/souffle_on_kafka.sh`.
    
- `script-to-run-experiments.sh`
    - Runs the experiments, using Docker compose commands.
    - A user's AWS credentials are written to a `.env` file; the script then changes to the directory of the `.env` file, so that calls to `docker-compose` use the contents of this file to populate the environment variables.
    - Each experiment generates a log file in `s3://souffle-on-kafka/output/log`; we check for the existence of this log file in the S3 bucket to signal an experiment's termination.
    
- `script-to-analyse-logs.sh`
    - Produces a `metrics.csv` from the logs in `~/.souffle/output/log`.
    - This uses the code in `./kafka/log-parser`.
    - Note that the log files must be synced from the S3 bucket.
    
- `script-to-generate-results.sh`
    - Produces results from the `metrics.csv` file generated by `script-to-analyse-results.sh`.
    - This uses the `./kafka/generate-results.dl` Datalog program for analysis.
    

## Results

Results are obtained from a log file generated by an experiment.

The log file is generated to the S3 bucket at the conclusion of an experiment.

The path of the log file is `s3://souffle-on-kafka/output/log/<experiment-name>.log`, where `<experiment-name>` is the unique name of the experiment. 

In particular, the experiment name will be the same as the directory containing the `docker-compose.yml` file that defines the experiment, in the S3 bucket at `s3://souffle-on-kafka/docker-compose/`.

The schema of log messages is described by the following grammar.

~~~
LOG_MESSAGE := HEAD,BODY
HEAD := <stratum-name>,<message-index>,<timestamp>
BODY := beginBashScript
    | endBashScript
    | beginSouffleProgram
    | endSouffleProgram
    | downloadInput,<line-count>
    | uploadOutput,<line-count>
    | printMetadata,<benchmark-program>,<data-type>,<split-size>,<join-type>,<kafka-mode>,<evaluation-algorithm>,<dataset-name>,<thread-count>,<unique-id>
    | beginClient
    | endClient
    | beginProduction,<topic-name>
    | endProduction,<topic-name>
    | beginConsumption,<topic-name>
    | endConsumption,<topic-name>
    | beginProduce,<topic-name>,<payload-size>,<payload-type-size>
    | endProduce,<topic-name>
    | beginConsume,<topic-name>
    | endConsume,<topic-name>,<payload-size>,<payload-type-size>
    | beginPollProducer
    | endPollProducer
    | beginPollConsumer
    | endPollConsumer
~~~

A log message is composed of a `HEAD` and a `BODY`.

The `HEAD` of a log message consists of each of the following fields.

- `<stratum-name>` is the name of the stratum emitting this message, or "none". Note that the -2 stratum is responsible for reading and producing input files, while the -3 stratum does the same for output files.
- `<message-index>` is the index or count of the current message.
- `<timestamp>` is the timestamp in milliseconds since the last message.

The `BODY` of a log message contains information about the operation which cased the message to be emitted.

The `BODY` of a log message starts with the name of the operation to which the message corresponds, followed by a number of values involved in that operation.

The following message bodies are emitted by operations from the bash script that is responsible for managing execution of the Souffle program.

- `printMetadata,<benchmark-program>,<data-type>,<split-size>,<join-type>,<kafka-mode>,<evaluation-algorithm>,<dataset-name>,<thread-count>,<unique-id>`
    - Emmitted immediately after the bash script managing the Souffle program execution begins. This effectively gives the input parameters of the experiment.
    - `<benchmark-program>` is the name of the benchmark program in use.
    - `<data-type>` is the Souffle data type, either `number` or `symbol`.
    - `<split-size>` is the number of splits in the Souffle program for the IDB relation.
    - `<join-type>` is the type of join used for the splits in the Souffle program.
    - `<kafka-mode>` is the Kafka mode, either `no-kafka` for no use of Kafka, `one-kafka` for running each Souffle program strata as a separate process in the same Docker container, or `many-kafka` for running each Souffle program strata in a separate Docker container.
    - `<evaluation-algorithm>` is the evaluation algorithm used, either `SNE`, `GSNE`, `GPSNE`, or `GPCSNE`.
    - `<dataset-name>` is the name of the dataset used in the experiment.
    - `<thread-count>` is the number of threads used by the Souffle program.
    - `<unique-id>` is the unique identifier of this experiment.
- `beginBashScript`
    - Emmitted immediately after the `printMetadata` message is emitted.
- `endBashScript`
    - Emitted immediately before the bash script managing the Souffle program execution ends.
- `beginSouffleProgram`
    - Emitted immediately before execution of the Souffle program.
- `endSouffleProgram`
    - Emitted immediately after the execution of the Souffle program.
- `downloadInput,<line-count>`
    - Emitted immediately after the input for the Souffle program has been downloaded.
    - `<line-count>` is the number of lines in the downloaded file (or files).
- `uploadOutput,<line-count>`
    - Emitted immediately after the output for the Souffle program has been uploaded.
    - `<line-count>` is the number of lines in the uploaded file (or files).

The following message bodies are mmitted by operations from within the execution of the Souffle program itself.

- `beginClient`
    - Emitted immediately after the Souffle program's client is connected to Kafka.
- `endClient`
    - Emitted immediately before the Souffle program's client terminates its connection, along with the program itself.
- `beginProduction,<topic-name>`
    - Emitted immediately after instantiation of a producer for a given topic.
    - `<topic-name>` is the name of the topic on which to produce.
- `endProduction,<topic-name>`
    - Emmitted immediately after terminating all production on a given topic.
    - `<topic-name>` is the name of the topic.
- `beginConsumption,<topic-name>`
    - Emitted immediately after instantiation of a consumer for a given topic.
    - `<topic-name>` is the name of the topic on which to consume.
- `endConsumption,<topic-name>`
    - Emmitted immediately after terminating all consumption on a given topic.
    - `<topic-name>` is the name of the topic.
- `beginProduce,<topic-name>,<payload-size>,<payload-type-size>`
    - Emitted before production of a message on given topic.
    - `<topic-name>` The name of the topic.
    - `<payload-size>` Number of elements in the payload vector.
    - `<payload-type-size>` Size in bytes of each element in the payload vector.
- `endProduce,<topic-name>`
    - Emitted after production of a message on a given topic. 
    - `<topic-name>` is the name of the topic.
- `beginConsume,<topic-name>`
    - Emitted before consumption of a message on a given topic. 
    - `<topic-name>` is the name of the topic.
- `endConsume,<topic-name>,<payload-size>,<payload-type-size>`
    - Emitted after consumption of a message on given topic.
    - `<topic-name>` is the name of the topic.
    - `<payload-size>` is the number of elements in the payload vector.
    - `<payload-type-size>` is the size in bytes of each element in the payload vector.
- `beginPollProducer`
    - Emitted after polling of producer to process outbound messages has begun.
- `endPollProducer`
    - Emitted after polling of producer to process outbound messages has ended.
- `beginPollConsumer`
    - Emitted after polling of consumer to process inbound messages has begun.
- `endPollConsumer`
    - Emitted after polling of consumer to process inbound messages has ended.

## Metrics

Metrics are collected from the results in the log files.
All metrics are placed in one CSV file (e.g. `metrics.csv`).
This csv file can be imported as a table into a spreadsheet.
It has all aggregation already computed, so no formula are necessary within the spreadsheet.

Each line of a metrics file is composed of the following

~~~
LINE := HEAD,BODY
HEAD := <benchmark-program>,<data-type>,<split-size>,<join-type>,<kafka-mode>,<evaluation-algorithm>,<dataset-name>,<thread-count>,<unique-id>
BODY := communicationTime,<stratum-index>,<runtime>
    | computationTime,<stratum-index>,<runtime>
    | runTime,<stratum-index>,<runtime>
    | totalCommunicationTime,<runtime>
    | totalComputationTime,<runtime>
    | totalRunTime,<runtime>
    | inputSize,<tuple-count>
    | outputSize,<tuple-count>
    | stringBytesProduced,<stratum-index>,<byte-count>
    | relationTuplesProduced,<relation-name>,<tuple-count>
    | totalStringBytesProduced,<byte-count>
    | totalRelationTuplesProduced,<tuple-count>
~~~

Metrics are to be computed for each log file generated by the experiments.

Each of the metrics to be computed is described below, with respect to the messages in the current log file.

For the `HEAD`...

- `<benchmark-program>,<data-type>,<split-size>,<join-type>,<kafka-mode>,<evaluation-algorithm>,<dataset-name>,<thread-count>,<unique-id>`
    - This is obtained from the single `printMetadata` message in the log file.

For the `BODY`...

- `communicationTime,<stratum-index>,<runtime>`
    - Per stratum sum of the `<timestamp>` fields for each `beginClient`, `endProduction`, `endConsumption`, `endProduce`, `endConsume`, `endPollProducer`, `endPollConsumer` log messages.
- `computationTime,<stratum-index>,<runtime>`
    - Per stratum value of `runTime` minus `communicationTime`.
- `runTime,<stratum-index>,<runtime>`
    - Per stratum value of the `<timestamp>` field of `endSouffleProgram`.
- `totalCommunicationTime,<runtime>`
    - Sum of `communicationTime` over all strata.
- `totalComputationTime,<runtime>`
    - Sum of `computationTime` over all strata.
- `totalRunTime,<runtime>`
    - Sum of `runTime` over all strata.
- `inputSize,<tuple-count>`
    - The `<line-count>` field of `downloadInput` log message.
- `outputSize,<tuple-count>`
    - The `<line-count>` field of `uploadOutput` log message.
- `stringBytesProduced,<stratum-index>,<byte-count>`
    - Per stratum sum of `<payload-size>` fields of all `beginProduce` messages
    having a `<topic-name>` field which is an integer or 'none'.
- `relationTermsProduced,<relation-name>,<tuple-count>`
    - Sum of `<payload-size>` fields of all `beginProduce` messages
    having a `<topic-name>` of `<relation-name>`.
- `totalStringBytesProduced,<byte-count>`
    - Sum of `stringBytesProduced` over all strata.
- `totalRelationTuplesProduced,<tuple-count>`
    - Sum of `relationsTuplesProduced` over all relations.
    
**Note: All negative values for `<stratum-index>` must be changed to 'none' before processing.**
    
### Example

As an example, consider the following log file:

~~~
none,0,15,printMetadata,NR,number,0,none,no-kafka,SNE,1,complete-graph-4,1,1603605921
none,1,301,beginBashScript
none,2,2875,downloadInput,12
none,3,36,beginSouffleProgram
none,4,67,endSouffleProgram
none,5,1223,uploadOutput,16
none,6,37,endBashScript
~~~

This is the log file generated in the `output/log` directory by an experiment.

The generation of the metrics for this log are as follows.


First, we get the `HEAD` from the log message

~~~
none,0,15,printMetadata,NR,number,0,none,no-kafka,SNE,1,complete-graph-4,1,1603605921
~~~

This gives us a `HEAD` value of `NR,number,0,none,no-kafka,SNE,1,complete-graph-4,1,1603605921`.

We then continue processing the log file.

To save space, we use `HEAD` to mean `NR,number,0,none,no-kafka,SNE,1,complete-graph-4,1,1603605921`.

The log message

~~~
none,4,67,endSouffleProgram
~~~

gives us the metric

~~~
HEAD,totalRuntime,67
~~~

The log message 

~~~
none,2,2875,downloadInput,12
~~~

gives the metric

~~~
HEAD,inputSize,12
~~~

and from 

~~~
none,5,1223,uploadOutput,16
~~~

we obtain 

~~~
HEAD,outputSize,16
~~~

The final set of metrics generated by the program are:

~~~
NR,number,0,none,no-kafka,SNE,1,complete-graph-4,1,1603605921,totalRuntime,67
NR,number,0,none,no-kafka,SNE,1,complete-graph-4,1,1603605921,inputSize,12
NR,number,0,none,no-kafka,SNE,1,complete-graph-4,1,1603605921,outputSize,16
~~~

with `HEAD` now replaced by its value.


Notice that many of the metric types (e.g. `stringBytesProduced`, `communicationTime`) do not appear here.

This is because there is no messages in the log file from which they can be computed.

In general, all metrics will be generated if an experiment uses Kafka, but only the three in this example will be generated if it does not.

A log file uses Kafka unless it contains `no-kafka` in its `printMetadata` log message.


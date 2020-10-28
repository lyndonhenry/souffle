package au.edu.sydney.logparser;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.tuple.Pair;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.math.BigDecimal;
import java.nio.charset.Charset;
import java.util.*;

public class Main {

    private static Logger logger = LoggerFactory.getLogger(Main.class);
    private static final String METRICS_FILE = "metrics.csv";

    public static void main(String[] args) throws IOException {
	    final String path = args[0];

        FileUtils.deleteQuietly(new File(METRICS_FILE));

	    // iterate log files
        final File[] logFiles = new File(path).listFiles((file, s) -> s.endsWith(".log"));
        Arrays.stream(logFiles).forEach(Main::parseFile);

        logger.info("DONE");

        // print metrics at the end
        FileUtils.readLines(new File(METRICS_FILE), Charset.defaultCharset()).forEach(System.out::println);
    }

    private static void parseFile(File file) {
        logger.info("Analysing {}", file);

//        final Table<String, String, BigDecimal> metrics = HashBasedTable.create();
        // The Key is [metrics-event,strata-name], value is either a timestamp or a counter
        final Map<Pair<String, String>, BigDecimal> metrics = new HashMap<>();
        // indicator which stratas are under processing.
        final Map<String, Boolean> runningStrata = new HashMap<>();
        final String emptyKey = "<EMPTY>";
        final Set<String> strataNames = new HashSet<>();
        final Set<String> topicNames = new HashSet<>();
        String head = emptyKey;

        try {
            final List<String> lines = FileUtils.readLines(file, Charset.defaultCharset());
            for (String line: lines) {
                String[] parts = line.split(",");

                String stratumName = parts[0];     // TODO - is stratumNme and stratumIndex the same?
                String messageIndex = parts[1];
                BigDecimal value = new BigDecimal(parts[2]);
                String messageType = parts[3];

                strataNames.add(stratumName);

                switch (messageType) {
                    case "printMetadata" :
                        head = line.substring(line.lastIndexOf("printMetadata,") + "printMetadata,".length(), line.length() - 1);
                        break;

                        // Communication Time calculation
                        // Compute as SUM of all timestamps for following messages
                    case "beginClient" :
                        runningStrata.put(stratumName, true);
                    case "endProduction" :
                    case "endConsumption" :
                    case "endProduce" :
                    case "endConsume" :
                    case "endPollProducer" :
                    case "endPollConsumer" :
                        addValue(metrics, "communicationTime", stratumName, value);
                        break;
                        // Runtime - all messages between beginClient and endClient
                    case "endSouffleProgram" :
                        addValue(metrics, "computationTime", stratumName, value);
                        break;
                    case "endClient" :
                        addValue(metrics, "runTime", stratumName, value); // last inclusive runTime
                        runningStrata.put(stratumName, false);
                        break;
                    case "downloadInput" :
                        addValue(metrics, "inputSize", emptyKey, value);
                        break;
                    case "uploadOutput" :
                        addValue(metrics, "outputSize", emptyKey, value);
                        break;
                    case "beginProduce" :
                        String topicName = parts[4];
                        BigDecimal payload = new BigDecimal(parts[5]);
                        topicNames.add(topicName);
                        addValue(metrics, "stringBytesProduced", topicName, payload);
                        addValue(metrics, "relationTermsProduced", topicName, BigDecimal.ONE);  // TODO ???
                        break;
                }

                // if this strata is running, add timestamp
                if (runningStrata.getOrDefault(stratumName, false)) {
                    addValue(metrics, "runTime", stratumName, value);
                }

            }

            // Post computations for all strata names
            for (String name: strataNames) {
                BigDecimal communicationTime = metrics.getOrDefault(Pair.of("communicationTime", name), BigDecimal.ZERO);
                BigDecimal runTime = metrics.getOrDefault(Pair.of("runTime", name), BigDecimal.ZERO);

                // computationTime - ADD (runTime-communicationTime)
                Pair<String, String> computationTimeKey = Pair.of("computationTime", name);
                BigDecimal computationTime = runTime.subtract(communicationTime);
                metrics.compute(computationTimeKey, (k, v) -> (v == null) ? computationTime : v.add(computationTime));

                // totalCommunicationTime - ADD current communicationTime
                addValue(metrics, "totalCommunicationTime", emptyKey, communicationTime);
                // totalComputationTime - ADD current computationTime
                addValue(metrics, "totalComputationTime", emptyKey, metrics.getOrDefault(computationTimeKey, BigDecimal.ZERO));
                // totalRunTime
                addValue(metrics, "totalRunTime", emptyKey, runTime);
            }

            // post computation for all topic names
            for (String topic : topicNames) {
                // totalStringBytesProduced
                BigDecimal stringBytesProduced = metrics.getOrDefault(Pair.of("stringBytesProduced", topic), BigDecimal.ZERO);
                addValue(metrics, "totalStringBytesProduced", emptyKey, stringBytesProduced);
                // totalRelationTuplesProduced
                BigDecimal relationsTuplesProduced = metrics.getOrDefault(Pair.of("relationTermsProduced", topic), BigDecimal.ZERO);
                addValue(metrics, "totalRelationTuplesProduced", emptyKey, relationsTuplesProduced);
            }

            final List<String> resultStrings = new ArrayList<>();
            for (Pair<String, String> key : metrics.keySet()) {
                BigDecimal val = metrics.get(key);
//                if (!BigDecimal.ZERO.equals(val)) {
                    String line = head + "," + key.getLeft();
                    if (!emptyKey.equals(key.getRight())) line += "," + key.getRight();
                    line += "," + val;
                    resultStrings.add(line);
//                }
            }

            Collections.sort(resultStrings);
            FileUtils.writeLines(new File(METRICS_FILE), resultStrings, true);

        } catch (Exception e) {
            logger.error("Error to parse file {}", file, e);
        }
    }

    private static void addValue(
            final Map<Pair<String, String>, BigDecimal> metrics,
            final String event,
            final String stratumName,
            final BigDecimal value) {

        Pair<String, String> key = Pair.of(event, stratumName);
        metrics.compute(key, (k, v) -> (v == null) ? value : v.add(value));
    }

}

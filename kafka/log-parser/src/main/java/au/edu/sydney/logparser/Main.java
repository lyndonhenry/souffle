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
    private static String METRICS_FILE = "metrics.csv";

    public static void main(String[] args) throws IOException {
	    final String path = args[0];
	    if (args.length > 1) METRICS_FILE = args[1];

        FileUtils.deleteQuietly(new File(METRICS_FILE));

	    // iterate log files
        final File[] logFiles = new File(path).listFiles((file, s) -> s.endsWith(".log"));
        Arrays.stream(logFiles).forEach(Main::parseFile);

        logger.info("DONE results stored in {}", METRICS_FILE);

        // print metrics at the end
//        FileUtils.readLines(new File(METRICS_FILE), Charset.defaultCharset()).forEach(System.out::println);
    }

    private static void parseFile(File file) {
        logger.info("Analysing {}", file);

        // The Key is [metrics-event,strata-name], value is either a timestamp or a counter
        final Map<Pair<String, String>, BigDecimal> metrics = new HashMap<>();
        final String emptyKey = "<EMPTY>";
        final Set<String> strataNames = new HashSet<>();
        final Set<String> topicNames = new HashSet<>();
        String head = emptyKey;

        try {
            final List<String> lines = FileUtils.readLines(file, Charset.defaultCharset());
            for (String line: lines) {
                String[] parts = line.split(",");

                String stratumName = convertNegativeToNone(parts[0]);
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
                    case "endProduction" :
                    case "endConsumption" :
                    case "endProduce" :
                    case "endConsume" :
                    case "endPollProducer" :
                    case "endPollConsumer" :
                        addValue(metrics, "communicationTime", stratumName, value);
                        break;
                    case "endSouffleProgram" :
                        addValue(metrics, "runTime", stratumName, value);
                        break;
                    case "endClient" :
                        break;
                    case "downloadInput" :
                        addValue(metrics, "inputSize", emptyKey, value);
                        break;
                    case "uploadOutput" :
                        addValue(metrics, "outputSize", emptyKey, value);
                        break;
                    case "beginProduce" :
                        String topicName = convertNegativeToNone(parts[4]);
                        BigDecimal payload = new BigDecimal(parts[5]);
                        topicNames.add(topicName);

                        // a bit stupid way to check the topicName is a number
                        try {
                            // only when topicName == stratumName
                            if ("none".equals(topicName)) {
                                addValue(metrics, "stringBytesProduced", topicName, payload);
                            } else {
                                addValue(metrics, "stringBytesProduced", Integer.parseInt(topicName) + "", payload);
                            }
                        } catch (NumberFormatException e) {
                            // only when topicName == stratumName
                            addValue(metrics, "relationTermsProduced", topicName, payload);
                        }

                        break;
                }
            }

            // Post computations for all strata names
            for (String name: strataNames) {
                BigDecimal communicationTime = metrics.getOrDefault(Pair.of("communicationTime", name), BigDecimal.ZERO);
                BigDecimal runTime = metrics.getOrDefault(Pair.of("runTime", name), BigDecimal.ZERO);

                addValue(metrics, "computationTime", name, runTime.subtract(communicationTime));
                // totalCommunicationTime - ADD current communicationTime
                addValue(metrics, "totalCommunicationTime", emptyKey, communicationTime);
                // totalComputationTime - ADD current computationTime
                Pair<String, String> computationTimeKey = Pair.of("computationTime", name);
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

    private static String convertNegativeToNone(String stratumName) {
        String res = stratumName;
        try {
            int n = Integer.parseInt(stratumName);
            if (n < 0) {
                res = "none";
            }
        } catch (NumberFormatException e) {
            res = stratumName;  // stratum name
        }

        return res;
    }

}

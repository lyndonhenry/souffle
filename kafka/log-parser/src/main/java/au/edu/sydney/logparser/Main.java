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

    private static final Map<String, String> EVENTS_MAPPING = new HashMap<>();
    static {
        EVENTS_MAPPING.put("beginClient", "communicationTime");
        EVENTS_MAPPING.put("endProduction", "communicationTime");
        EVENTS_MAPPING.put("endConsumption", "communicationTime");
        EVENTS_MAPPING.put("endProduce", "communicationTime");
        EVENTS_MAPPING.put("endConsume", "communicationTime");
        EVENTS_MAPPING.put("endPollProducer", "communicationTime");
        EVENTS_MAPPING.put("endPollConsumer", "communicationTime");

        EVENTS_MAPPING.put("endSouffleProgram", "computationTime");

        EVENTS_MAPPING.put("beginClient", "runTime");       // TODO ??
        EVENTS_MAPPING.put("endClient", "runTime");

        EVENTS_MAPPING.put("downloadInput", "inputSize");

        EVENTS_MAPPING.put("uploadOutput", "outputSize");

        EVENTS_MAPPING.put("beginProduce", "stringBytesProduced");  // TODO ??

        EVENTS_MAPPING.put("beginProduce", "relationTermsProduced");    // TODO ??
    }

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

        final Map<Pair<String, String>, BigDecimal> metrics = new HashMap<>();
        final String emptyKey = "<EMPTY>";
        String head = emptyKey;

        try {
            final List<String> lines = FileUtils.readLines(file, Charset.defaultCharset());
            for (String line: lines) {
                String[] parts = line.split(",");

                String stratumNname = parts[0];     // TODO - is stratumNme and stratumIndex the same?
                String messageIndex = parts[1];
                BigDecimal timestamp = new BigDecimal(parts[2]);
                String messageType = parts[3];
                switch (messageType) {
                    case "printMetadata" :
                        head = line.substring(line.lastIndexOf("printMetadata,") + "printMetadata,".length(), line.length() - 1);
                        break;

                        // Communication Time calculation
                    case "beginClient" :
                    case "endProduction" :
                    case "endConsumption" :
                    case "endProduce" :
                    case "endConsume" :
                    case "endPollProducer" :
                    case "endPollConsumer" :
                        Pair<String, String> key = Pair.of("communicationTime", stratumNname);
                        metrics.compute(key, (k, v) -> (v == null) ? timestamp : v.add(timestamp));     // TODO - can we just ADD the timestamp ??
                        break;

                }
            }

            final List<String> resultStrings = new ArrayList<>();
            for (Pair<String, String> key : metrics.keySet()) {
                BigDecimal val = metrics.get(key);
                String line = head + "," + key.getLeft();
                if (!emptyKey.equals(key.getRight())) line += "," + key.getRight();
                line += "," + val;
                resultStrings.add(line);
            }

            FileUtils.writeLines(new File(METRICS_FILE), resultStrings, true);

        } catch (Exception e) {
            logger.error("Error to parse file {}", file, e);
        }
    }


}

/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2013, 2015, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file CompiledOptions.h
 *
 * A header file offering command-line option support for compiled
 * RAM programs.
 *
 ***********************************************************************/

#pragma once

#include "Util.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef _OPENMP
#include <omp.h>
#endif

namespace souffle {

/**
 * A utility class for parsing command line arguments within generated
 * query programs.
 */
class CmdOptions {
protected:
    /**
     * source file
     */
    std::string src;

    /**
     * fact directory
     */
    std::string input_dir;

    /**
     * output directory
     */
    std::string output_dir;

    /**
     * profiling flag
     */
    bool profiling;

    /**
     * profile filename
     */
    std::string profile_name;

    /**
     * number of threads
     */
    size_t num_jobs;

    /**
     * index of stratum to be executed
     */
    size_t stratumIndex;

    /**
     * Extra options, currently only used by Kafka engine.
     * These set global configuration properties in the librdkafka library.
     * @see Kafka.h for more information
     */
    std::unordered_map<std::string, std::string> extraOptions;

public:
    // all argument constructor
    CmdOptions(const char* s, const char* id, const char* od, bool pe, const char* pfn, size_t nj,
            size_t si = (size_t)-1,
            std::unordered_map<std::string, std::string> eo = std::unordered_map<std::string, std::string>())
            : src(s), input_dir(id), output_dir(od), profiling(pe), profile_name(pfn), num_jobs(nj),
              stratumIndex(si), extraOptions(eo) {}

    /**
     * get source code name
     */
    const std::string& getSourceFileName() const {
        return src;
    }

    /**
     * get input directory
     */
    const std::string& getInputFileDir() const {
        return input_dir;
    }

    /**
     * get output directory
     */
    const std::string& getOutputFileDir() const {
        return output_dir;
    }

    /**
     * is profiling switched on
     */
    bool isProfiling() const {
        return profiling;
    }

    /**
     * get filename of profile
     */
    const std::string& getProfileName() const {
        return profile_name;
    }

    /**
     * get number of jobs
     */
    size_t getNumJobs() const {
        return num_jobs;
    }

    /**
     * get index of stratum to be executed
     */
    size_t getStratumIndex() const {
        return stratumIndex;
    }

    /**
     * get extra options
     */
    const std::unordered_map<std::string, std::string>& getExtraOptions() const {
        return extraOptions;
    }

    /**
     * Parses the given command line parameters, handles -h help requests or errors
     * and returns whether the parsing was successful or not.
     */
    bool parse(int argc, char** argv) {
        // get executable name
        std::string exec_name = "analysis";
        if (argc > 0) {
            exec_name = argv[0];
        }

        // local options
        std::string fact_dir = input_dir;
        std::string out_dir = output_dir;

// avoid warning due to Solaris getopt.h
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
        // long options
        option longOptions[] = {{"facts", true, nullptr, 'F'}, {"output", true, nullptr, 'D'},
                {"profile", true, nullptr, 'p'}, {"jobs", true, nullptr, 'j'}, {"index", true, nullptr, 'i'},
                {"extra", true, nullptr, 'X'},
                // the terminal option -- needs to be null
                {nullptr, false, nullptr, 0}};
#pragma GCC diagnostic pop

        // check whether all options are fine
        bool ok = true;

        int c; /* command-line arguments processing */
        while ((c = getopt_long(argc, argv, "D:F:hp:j:i:X:", longOptions, nullptr)) != EOF) {
            switch (c) {
                /* Fact directories */
                case 'F':
                    if (!existDir(optarg)) {
                        printf("Fact directory %s does not exists!\n", optarg);
                        ok = false;
                    }
                    fact_dir = optarg;
                    break;
                /* Output directory for resulting .csv files */
                case 'D':
                    if (*optarg && !existDir(optarg)) {
                        printf("Output directory %s does not exists!\n", optarg);
                        ok = false;
                    }
                    out_dir = optarg;
                    break;
                case 'p':
                    if (!profiling) {
                        std::cerr << "\nError: profiling was not enabled in compilation\n\n";
                        printHelpPage(exec_name);
                        exit(1);
                    }
                    profile_name = optarg;
                    break;
                case 'j':
#ifdef _OPENMP
                    if (std::string(optarg) == "auto") {
                        num_jobs = 0;
                    } else {
                        int num = atoi(optarg);
                        if (num > 0) {
                            num_jobs = num;
                        } else {
                            std::cerr << "Invalid number of jobs [-j]: " << optarg << "\n";
                            ok = false;
                        }
                    }
#else
                    std::cerr << "\nWarning: OpenMP was not enabled in compilation\n\n";
#endif
                    break;
                case 'i':
                    stratumIndex = (size_t)std::stoull(optarg);
                    break;
                case 'X': {
                    const auto optargString = std::string(optarg);
                    auto key = std::string();
                    auto value = std::string();
                    auto* current = &key;
                    for (const auto character : optargString) {
                        switch (character) {
                            case '=': {
                                current = &value;
                                break;
                            }
                            default: {
                                (*current) += character;
                                break;
                            }
                        }
                    }
                    extraOptions.insert({key, value});
                } break;
                default:
                    printHelpPage(exec_name);
                    return false;
            }
        }

        // update member fields
        input_dir = fact_dir;
        output_dir = out_dir;

        // return success state
        return ok;
    }

private:
    /**
     * Prints the help page if it has been requested or there was a typo in the command line arguments.
     */
    void printHelpPage(const std::string& exec_name) const {
        std::cerr << "====================================================================\n";
        std::cerr << " Datalog Program: " << src << "\n";
        std::cerr << " Usage: " << exec_name << " [OPTION]\n\n";
        std::cerr << " Options:\n";
        std::cerr << "    -D <DIR>, --output=<DIR>     -- Specify directory for output relations\n";
        std::cerr << "                                    (default: " << output_dir << ")\n";
        std::cerr << "                                    (suppress output with \"\")\n";
        std::cerr << "    -F <DIR>, --facts=<DIR>      -- Specify directory for fact files\n";
        std::cerr << "                                    (default: " << input_dir << ")\n";
        if (profiling) {
            std::cerr << "    -p <file>, --profile=<file>  -- Specify filename for profiling\n";
            std::cerr << "                                    (default: " << profile_name << ")\n";
        }
#ifdef _OPENMP
        std::cerr << "    -j <NUM>, --jobs=<NUM>       -- Specify number of threads\n";
        if (num_jobs > 0) {
            std::cerr << "                                    (default: " << num_jobs << ")\n";
        } else {
            std::cerr << "                                    (default: auto)\n";
        }
#endif
        std::cerr << "    -i <N>, --index=<N>          -- Specify index of stratum to be executed\n";
        std::cerr << "                                    (or each in order if omitted)\n";
        std::cerr << "    -X <KEY>=<VAL>, --extra=<KEY>=<VAL> -- Specify extra options\n";
        std::cerr << "    -h                           -- prints this help page.\n";
        std::cerr << "--------------------------------------------------------------------\n";
        std::cout << " Copyright (c) 2016-20 The Souffle Developers." << std::endl;
        std::cout << " Copyright (c) 2013-16 Oracle and/or its affiliates." << std::endl;
        std::cerr << " All rights reserved.\n";
        std::cerr << "====================================================================\n";
    }

    /**
     *  Check whether a file exists in the file system
     */
    inline bool existFile(const std::string& name) const {
        struct stat buffer;
        if (stat(name.c_str(), &buffer) == 0) {
            if ((buffer.st_mode & S_IFREG) != 0) {
                return true;
            }
        }
        return false;
    }

    /**
     *  Check whether a directory exists in the file system
     */
    bool existDir(const std::string& name) const {
        struct stat buffer;
        if (stat(name.c_str(), &buffer) == 0) {
            if ((buffer.st_mode & S_IFDIR) != 0) {
                return true;
            }
        }
        return false;
    }
};

}  // end of namespace souffle

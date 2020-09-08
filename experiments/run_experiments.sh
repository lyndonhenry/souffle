#!/bin/bash

#
# This script can be used to run the experiments.
# Right now, it is just a starting point. 
#
# The following are TODO's:
#   1. Run each Datalog program on the example dataset.
#   2. Run each Datalog program under all system configurations on the example 
#   datasets (see notes.pdf on the Overleaf for more on the system 
#   configurations).
#   3. Collect experimental datasets, run each Datalog program under all 
#   configurations on them.
#
# Note that:
#   - More Datalog programs will be added in future, but will typically be 
#   compatible with existing datasets.
#   - Any relation marked .input in a .dl file must have a .facts file in the 
#   directory passed to -F.
#   - Any relation marked .output in a .dl file will generate a .csv file in 
#   the directory passed to -D.
#   - One .fact file may be used by multiple programs.
#   - More .dl files will be added in the future, but will typically use 
#   relation names compatible with existing .fact files.

# this just ensures that the output directory exists
mkdir -p experiments/csv

# this runs Souffle on one of the Datalog programs, passing the -F and -D 
# directories as explained in the comments at the top of this script
./src/souffle -F./experiments/facts -D./experiments/csv ./experiments/LeftLinearTransitiveClosure.dl 


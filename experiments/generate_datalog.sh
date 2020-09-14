#!/bin/bash

set -ouex pipefail

#
# This script generates Datalog files for the experiments.
#

# This function generate all simple benchmarks.

function generate_simple_benchmarks() {

# Left-recursive Reachability

cat > LR.dl << EOF
.decl E(a:number,b:number)
.input E
.decl P(a:number,b:number)
.output P
P(a,b):-E(a,b).P(a,b):-E(a,x),P(x,b).
EOF

# Right-recursive Reachability

cat > RR.dl << EOF
.decl E(a:number,b:number)
.input E
.decl P(a:number,b:number)
.output P
P(a,b):-E(a,b).P(a,b):-P(a,x),E(x,b).
EOF

# Non-linear Reachability

cat > NR.dl << EOF
.decl E(a:number,b:number)
.input E
.decl P(a:number,b:number)
.output P
P(a,b):-E(a,b).P(a,b):-P(a,x),P(x,b).
EOF

# Same Generation

cat > SG.dl << EOF
.decl E(a:number,b:number)
.input E
.decl S(a:number,b:number)
.output S
S(a,a):-E(a,_);E(_,a).S(a,b):-E(a,x),E(b,x),S(a,b).
EOF

# Reverse Same Generation

cat > RSG.dl << EOF
.decl E(a:number,b:number)
.input E
.decl S(a:number,b:number)
.output S
S(a,a):-E(a,_);E(_,a).S(a,b):-E(x,a),E(x,b),S(a,b).
EOF

# Triangle Counting

cat > TC.dl << EOF
.decl E(a:number,b:number)
.input E
.decl T(a:number,b:number,c:number)
.output T
T(a,b,c):-E(a,b),E(b,c),E(c,a).
EOF

# Strongly Connected Components
# Note: The input is not E here, but P from LR/RR/NR.

cat > SCC.dl << EOF
.decl P(a:number,b:number)
.input P
.decl S(a:number,b:number)
.output S
S(a,b):-P(a,b),P(b,a).
EOF

# Mutual Neighbours

cat > MN.dl << EOF
.decl E(a:number,b:number)
.input E
.decl M(a:number,b:number,c:number)
.output M
M(a,b,c):-E(a,c),E(b,c).
EOF

}

# This function generates a version of the NR benchmark using negation on one of the IDB relations.

function generate_negated_nr_benchmark() {

cat > NR-negated.dl << EOF
.decl E(a:number,b:number)
.input E
.decl P(a:number,b:number)
.decl N(a:number,b:number)
.decl V(a:number)
.output N
P(a,b):-E(a,b).P(a,b):-P(a,x),P(x,b).
V(a):-E(a,_);E(_,a).
N(a,b):-V(a),V(b),!P(a,b).
EOF

}

# This function generates a split/join version of the NR benchmark.

function generate_splitjoin_nr_benchmark() {

  # the parameters are the output file, the breadth of the split, and the type of the join

local FILE=${1}
local SIZE=${2}
local TYPE=${3}

  # initally, we generate code for the original input and output columns

cat > ${FILE} << EOF
.decl E(a:number,b:number)
.input E
.decl P(a:number,b:number)
.output P
EOF

  # then, we generate additional rules and relations for intermediates
  # we always split data on the first column of the EDB predicate

for ((i=0; i < ${SIZE}; ++i))
do
cat >> ${FILE} << EOF
.decl E_${i}(a:number,b:number)
E_${i}(a,b) :- E(a,b), a % ${SIZE} = ${i}.
.decl P_${i}(a:number,b:number)
P_${i}(a,b) :- E_${i}(a,b). P_${i}(a,b) :- P_${i}(a,x), P_${i}(x,b). 
EOF
done

# there are three choices of joiner, only two are implemented right now
# the FIRST type is a many-to-one joiner, where all P_i's are merged into P, and P computed
# the SECOND type joins across, with each P_i consuming from P_{i-1}, and P = P_{n - 1}
# the THIRD type will join in a tree-like manner, with P_{01} consuming from P_{0}, P_{1}, and P_{12} from P_{1}, P_{2}, etc.

case ${TYPE} in 
  FIRST)
    for ((i=0; i < ${SIZE}; ++i)) 
    do
cat >> ${FILE} << EOF
P(a,b) :- P_${i}(a,b).
EOF
    done
cat >> ${FILE} << EOF
P(a,b) :- P(a,x), P(x,b).
EOF
    ;;
  SECOND)
    for ((i=1; i < ${SIZE}; ++i)) 
    do
      j=$((i - 1))
cat >> ${FILE} << EOF
P_${i}(a,b) :- P_${j}(a,b).
EOF
    done
    k=$((SIZE - 1))
cat >> ${FILE} << EOF
P(a,b) :- P_${k}(a,b).
EOF
    ;;
  THIRD)
    echo "ERROR: NOT IMPLEMENTED!"
    ;;
esac

}

function main() {

  rm -rf ./experiments/*.dl

  cd experiments

  # generate all the simple benchmarks (no splits/joins)

  generate_simple_benchmarks

  # generate a split/join version of NR, with a breadth of 8, for join types FIRST and SECOND

  generate_splitjoin_nr_benchmark "NR-splitjoin-8-first.dl" 8 "FIRST"

  generate_splitjoin_nr_benchmark "NR-splitjoin-8-second.dl" 8 "SECOND"

  # create symbol typed versions of all files

  for FILE in *.dl
  do
    
    # change all number types to symbols
    sed 's/:number/:symbol/g' ${FILE} > symbol-${FILE}

    # convert strings to ord values for partitioning
    sed 's/a % n/ord(a) % n/g' ${FILE} > symbol-${FILE}

  done

  # ensure the input and output dirs exist

  rm -rf csv
  mkdir -p facts csv

  # generate debug report of each program for SCC graph analysis

  for FILE in *.dl
  do
    ../src/souffle -Ffacts -Dcsv -r${FILE}.html ${FILE} || :
  done

  cd -

}

main

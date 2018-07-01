#!/bin/bash

# NOTES
# - ensure return of null works for iprobe for symbol table thread
# - ensure joinThreads method workds for SymbolTable, must release lock before join
# - put most of Mpi.h in an Mpi.cpp, too much in header
# - do not use anonymous namespaces in Mpi.h, maybe ok in cpp file
# - do documentation for whole of Mpi.h class
# - make the debugging better, only do it where required, make an mpi.dl for analysis

# @TODO: the bug is that in duplicates, all executed sends are to rank 3 (ie stratum 2) -- stratum 2 should have one sent to it only!

set -x

function main() {
    if [ $# == 0 ]
    then
        ./utilities/git/hooks/pre-commit.sh
        export SOUFFLE_CONFS="-c -j8 -empi"
        ./bootstrap
        ./configure --enable-debug --enable-mpi
        make -j8
    else
        local cwd=./tests/testsuite.dir/${1}-${2}
        mkdir -p ./tests/testsuite.dir
        cp -r ./tests/${1}/${2} ${cwd}
        ./src/souffle -empi -j8 -g${cwd}/${2}.cpp -D${cwd} -F${cwd}/facts -r${cwd}/${2}.html ${cwd}/${2}.dl
        clang-format-4.0 -i ${cwd}/${2}.cpp
        # @TODO: output to stderr
        ./src/souffle -empi -j8 -c -D${cwd} -F${cwd}/facts ${cwd}/${2}.dl > ${cwd}/${2}.facts
        ./src/souffle -F${cwd} -D${cwd} ./tests/example/mpi/mpi.dl
    fi
}

main ${@}



#
#cat duplicates.err | \
# grep "\(Isend\|Recv\)" | \
# sed 's/= [0-9x]*\|Isend\|Recv/|&|/g' | \
# cut -d'|' -f4,10,18,20 | \
# sed 's/= //g' | \
# sed 's/|/\t/g' > message.facts
#
#cat > shouldRecvButDidNot.dl << EOF
#.decl message(rank: number, function: symbol, sourceOrDestination: number, tag: number)
#.input message
#
#.decl isendNotRecv(source: number, destination: number, tag: number)
#.output isendNotRecv
#
#isendNotRecv(s, d, t) :- message(s, "Isend", d, t), !message(d, "Recv", s, t).
#EOF
#

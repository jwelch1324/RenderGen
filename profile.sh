#!/bin/bash
CPUPROFILE=$1.prof ./$1 #DYLD_INSERT_LIBRARIES=/usr/local/Cellar/gperftools/2.9.1_1/lib/libprofiler.dylib ./$1
pprof --pdf $1 $1.prof > $1.pdf
echo "Profiling results: $1.pdf"

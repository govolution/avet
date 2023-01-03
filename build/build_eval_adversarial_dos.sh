#!/bin/bash

# print AVET logo
cat banner.txt

# include script containing the compiler var $win64_compiler
# you can edit the compiler in build/global_win64.sh
# or enter $win64_compiler="mycompiler" here
. build/global_win64.sh

# import feature construction interface
. build/feature_construction.sh



FILES="input/malware-filtered2/*"
N=3
practical_manipulation="full_dos"
population_size=100
section_size=5

for f in $FILES
do
    ((i=i%N)); ((i++==0)) && wait

    ( cp $f output/

    # generate adversarial example
    $genetic_optimizer -ss $section_size -pm $practical_manipulation -p $population_size output/$(basename "$f")

    # cleanup
    rm output/$(basename "$f")
    # cleanup_techniques 
    ) &
done

wait
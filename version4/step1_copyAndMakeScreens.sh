#!/bin/bash

mkdir testing
rm -r testing/*

for((i=0;i<3;i++));do
    cp -r origin testing/situ$i
    screen -dm MLEMsitu$i
done

IDAndItensities=("0 1e6" "1 1e6" "2 1e6")
for((i=0;i<3;i++));do
    AValue=${IDAndItensities[i]}
    sed -i "s/0 1e6/$AValue/g" testing/situ$i/parameters_sourcesGroundTruth.txt
done

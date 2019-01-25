#!/bin/bash

for((i=0;i<3;i++));do
    screen -x -S MLEMsitu$i -p 0 -X stuff "cd testing/situ$i/build/\n"
    screen -x -S MLEMsitu$i -p 0 -X stuff ". ../Run.sh\n"
done

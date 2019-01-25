#!/bin/bash

for((i=0;i<3;i++));do
    screen -r MLEMsitu$i -X quit
done

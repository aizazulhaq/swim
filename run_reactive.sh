#!/bin/bash

FILENAME=$(date +run_%Y%m%d_%H%M%S).pdf

cd ~/seams-swim/swim/simulations/swim_sa/
./run.sh reactive 0

cd ~/seams-swim/results
../swim/tools/plotResults.sh SWIM_SA reactive 0 $FILENAME

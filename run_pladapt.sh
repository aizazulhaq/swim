#!/bin/bash

FILENAME=$(date +run_%Y%m%d_%H%M%S)

cp ~/yaml/pladapt/*.yaml ~/yaml/

cd ~/seams-swim/swim/simulations/swim_sa/
./run.sh pladapt 0

cd ~/seams-swim/results
../swim/tools/plotResults.sh SWIM_SA pladapt 0 "${FILENAME}.pdf"

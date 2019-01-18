#!/bin/bash

FILENAME=$(date +run_%Y%m%d_%H%M%S)

cp ~/yaml/thallium/*.yaml ~/yaml/

cd ~/seams-swim/swim/simulations/swim_sa/
./run.sh thallium 0

cd ~/seams-swim/results
../swim/tools/plotResults.sh SWIM_SA thallium 0 "${FILENAME}.pdf"

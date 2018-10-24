#!/bin/sh
# Run this script from NS-3 project root directory (in Docker).

export NS_LOG="LAB4=debug|prefix_level"

X=$1
Y=$2
Z=$3
APP_DATA_RATE=$4

echo "Running with UE vectors: ($1, $2, $3) and app. data rate: $4 Mbps"

set -e
set -v

./waf --run "lab4-scenario \
	-x=$1 -y=$2 -z=$3 \
	--appDataRate=$4 \
	--outputPath=results/lab4/isotropic \
	--antennaType=IsotropicAntennaModel"
./waf --run "lab4-scenario \
	-x=$1 -y=$2 -z=$3 \
	--appDataRate=$4 \
	--outputPath=results/lab4/parabolic \
	--antennaType=ParabolicAntennaModel"
./waf --run "lab4-scenario \
	-x=$1 -y=$2 -z=$3 \
	--appDataRate=$4 \
	--outputPath=results/lab4/cosine \
	--antennaType=CosineAntennaModel"

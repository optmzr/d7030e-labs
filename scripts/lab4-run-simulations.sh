#!/bin/sh
# Run this script from NS-3 project root directory (in Docker).

export NS_LOG="LAB4=debug|prefix_level"

./waf --run "lab4-scenario --outputPath=results/lab4/isotropic --antennaType=IsotropicAntennaModel"
./waf --run "lab4-scenario --outputPath=results/lab4/parabolic --antennaType=ParabolicAntennaModel"
./waf --run "lab4-scenario --outputPath=results/lab4/cosine --antennaType=CosineAntennaModel"

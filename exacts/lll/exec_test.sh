#!/usr/bin/env bash

make

echo "small"

echo "r_30_40_30_20"
SECONDS=0
./lattice.out < ../../instances/new/r_30_40_30_20.input > r_30_40_30_20.lat
fplll r_30_40_30_20.lat > r_30_40_30_20.out 
./result.out 391 < r_30_40_30_20.out
t=$SECONDS
echo $t"s"

echo "r_40_40_30_20"
SECONDS=0
./lattice.out < ../../instances/new/r_40_40_30_20.input > r_40_40_30_20.lat
fplll r_40_40_30_20.lat > r_40_40_30_20.out 
./result.out 633 < r_40_40_30_20.out
t=$SECONDS
echo $t"s"

echo "r_60_10_50_5"
SECONDS=0
./lattice.out < ../../instances/new/r_60_10_50_5.input > r_60_10_50_5.lat
fplll r_60_10_50_5.lat > r_60_10_50_5.out 
./result.out 383 < r_60_10_50_5.out
t=$SECONDS
echo $t"s"

echo "r_80_8_60_8"
SECONDS=0
./lattice.out < ../../instances/new/r_80_8_60_8.input > r_80_8_60_8.lat
fplll r_80_8_60_8.lat > r_80_8_60_8.out 
./result.out 514 < r_80_8_60_8.out
t=$SECONDS
echo $t"s"


echo "medium"

echo "r_50_50_90_45"
SECONDS=0
./lattice.out < ../../instances/new/r_50_50_90_45.input > r_50_50_90_45.lat
fplll r_50_50_90_45.lat > r_50_50_90_45.out 
./result.out 1286 < r_50_50_90_45.out
t=$SECONDS
echo $t"s"

echo "r_75_35_100_50"
SECONDS=0
./lattice.out < ../../instances/new/r_75_35_100_50.input > r_75_35_100_50.lat
fplll r_75_35_100_50.lat > r_75_35_100_50.out 
./result.out 2080 < r_75_35_100_50.out
t=$SECONDS
echo $t"s"

echo "r_100_8_60_10"
SECONDS=0
./lattice.out < ../../instances/new/r_100_8_60_10.input > r_100_8_60_10.lat
fplll r_100_8_60_10.lat > r_100_8_60_10.out 
./result.out 865 < r_100_8_60_10.out
t=$SECONDS
echo $t"s"

echo "r_150_6_80_10"
SECONDS=0
./lattice.out < ../../instances/new/r_150_6_80_10.input > r_150_6_80_10.lat
fplll r_150_6_80_10.lat > r_150_6_80_10.out 
./result.out 1447 < r_150_6_80_10.out
t=$SECONDS
echo $t"s"



# echo "large"

# echo "r_200_20_80_40"
# ./lattice.out < ../../instances/new/r_200_20_80_40.input > r_200_20_80_40.lat
# fplll r_200_20_80_40.lat > r_200_20_80_40.out 
# ./result.out 8280 < r_200_20_80_40.out

# echo "r_300_15_100_50"
# ./lattice.out < ../../instances/new/r_300_15_100_50.input > r_300_15_100_50.lat
# fplll r_300_15_100_50.lat > r_300_15_100_50.out 
# ./result.out 14101 < r_300_15_100_50.out

# echo "r_400_2_40_15"
# ./lattice.out < ../../instances/new/r_400_2_40_15.input > r_400_2_40_15.lat
# fplll r_400_2_40_15.lat > r_400_2_40_15.out 
# ./result.out 3394 < r_400_2_40_15.out

# echo "r_600_1_50_10"
# ./lattice.out < ../../instances/new/r_600_1_50_10.input > r_600_1_50_10.lat
# fplll r_600_1_50_10.lat > r_600_1_50_10.out 
# ./result.out 3862 < r_600_1_50_10.out
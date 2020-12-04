#!/usr/bin/env bash

# echo "./exacts/cplex/main.out 0 < ./instances/small/r_40_40_15_5.input"
# ./exacts/cplex/main.out 0 < ./instances/small/r_40_40_15_5.input > ./outputs/r_40_40_15_5-cplex.dat
# echo "./exacts/cplex/main.out 1 < ./instances/small/r_40_40_15_5.input"
# ./exacts/cplex/main.out 1 < ./instances/small/r_40_40_15_5.input > ./outputs/r_40_40_15_5-cplex-lb.dat
# echo "./exacts/local-branching/main.out < ./instances/small/r_40_40_15_5.input"
# ./exacts/local-branching/main.out < ./instances/small/r_40_40_15_5.input > ./outputs/r_40_40_15_5-lb.dat

# echo "./exacts/cplex/main.out 0 < ./instances/small/r_45_25_15_10.input"
# ./exacts/cplex/main.out 0 < ./instances/small/r_45_25_15_10.input > ./outputs/r_45_25_15_10-cplex.dat
# echo "./exacts/cplex/main.out 1 < ./instances/small/r_45_25_15_10.input"
# ./exacts/cplex/main.out 1 < ./instances/small/r_45_25_15_10.input > ./outputs/r_45_25_15_10-cplex-lb.dat
# echo "./exacts/local-branching/main.out < ./instances/small/r_45_25_15_10.input"
# ./exacts/local-branching/main.out < ./instances/small/r_45_25_15_10.input > ./outputs/r_45_25_15_10-lb.dat

echo "./exacts/cplex/main.out 0 < ./generator/r_120_10_80_10.input"
./exacts/cplex/main.out 0 < ./generator/r_120_10_80_10.input > ./outputs/r_120_10_80_10-cplex.dat
echo "./exacts/cplex/main.out 1 < ./generator/r_120_10_80_10.input"
./exacts/cplex/main.out 1 < ./generator/r_120_10_80_10.input > ./outputs/r_120_10_80_10-cplex-lb.dat
echo "./exacts/local-branching/main.out < ./generator/r_120_10_80_10.input"
./exacts/local-branching/main.out < ./generator/r_120_10_80_10.input > ./outputs/r_120_10_80_10-lb.dat

echo "./exacts/cplex/main.out 0 < ./generator/r_150_5_60_5.input"
./exacts/cplex/main.out 0 < ./generator/r_150_5_60_5.input > ./outputs/r_150_5_60_5-cplex.dat
echo "./exacts/cplex/main.out 1 < ./generator/r_150_5_60_5.input"
./exacts/cplex/main.out 1 < ./generator/r_150_5_60_5.input > ./outputs/r_150_5_60_5-cplex-lb.dat
echo "./exacts/local-branching/main.out < ./generator/r_150_5_60_5.input"
./exacts/local-branching/main.out < ./generator/r_150_5_60_5.input > ./outputs/r_150_5_60_5-lb.dat

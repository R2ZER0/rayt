#!/bin/bash
set -euo pipefail

#g++ -o rayt -Wall --std=c++11 -g ./*.cpp
g++ -o rayt -Wall --std=c++11 -O3 -fopenmp ./*.cpp

#!/bin/bash
set -euo pipefail

g++ -o rayt -Wall --std=c++11 -ggdb ./*.cpp

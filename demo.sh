#!/usr/bin/env bash
set -e
if [ "$#" -lt 2 ]; then
  echo "Usage: $0 input.wav output.wav [cutoff_hz]"
  exit 1
fi
IN=$1; OUT=$2; FC=$3

# Ensure deps: libsndfile (system), pip packages (python)
echo "Compiling C++..."
g++ -O3 -std=c++17 simple_lp.cpp -lsndfile -o simple_lp

echo "Running and plotting..."
if [ -n "$FC" ]; then
  python3 run_and_plot.py "$IN" "$OUT" "$FC"
else
  python3 run_and_plot.py "$IN" "$OUT"
fi

echo "Done. Outputs: $OUT, before_after.png"

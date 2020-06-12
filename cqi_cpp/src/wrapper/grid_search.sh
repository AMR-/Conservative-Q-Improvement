#!/bin/bash

# Grid search
rm grid_search_results.txt
touch grid_search_results.txt

for c in 10 50 100 150 200 250
do
    python cqi_test.py --ucb True --c $c --grid_search True
done

# Find max reward and corresponding hparams line
awk 'BEGIN {max = max_line = -1000} {if ($5 ~ /^(\-[0-9]+)|([0-9]+)$/ && $5 > max) {max = $5; max_line = NR}} END {print max, max_line + 1}' grid_search_results.txt 

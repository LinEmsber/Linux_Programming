#/bin/bash

# A simple script for execute program to specify perf events

REPEAT_TIME=10

HARDWARE_EVENT="bus-cycles,cycles,instructions"

CACHE_EVENT="L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses,LLC-load-misses,LLC-loads,LLC-store-misses,LLC-stores,branch-load-misses,branch-loads,dTLB-load-misses,dTLB-loads,dTLB-store-misses,dTLB-stores,iTLB-load-misses,iTLB-loads"

perf stat -e "$HARDWARE_EVENT,$CACHE_EVENT" -r $REPEAT_TIME $1

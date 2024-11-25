#!/bin/bash

# Define variables
SERVER_IP="<server-ip>"
OUTPUT_FILE="iperf3_results.txt"
DURATION=10  # Duration of each test in seconds
INTERVAL=2    # Interval between tests in seconds

echo "Starting continuous iperf3 tests..." > $OUTPUT_FILE

# Infinite loop to run tests
while true; do
    echo "Running iperf3 test at $(date)" >> $OUTPUT_FILE
    iperf3 -c $SERVER_IP -t $DURATION >> $OUTPUT_FILE
    echo "Test complete. Waiting for $INTERVAL seconds..." >> $OUTPUT_FILE
    sleep $INTERVAL
done

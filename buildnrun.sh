if [[ $1 == *--mac* ]]; then
    ./build_mac.sh
else
    ./build.sh
fi
./bin/interrupts trace.txt vector_table.txt device_table.txt
cat execution.txt
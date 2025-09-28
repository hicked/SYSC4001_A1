if [[ $1 == *--mac* ]]; then
    ./build.sh --mac
else
    ./build.sh
fi
./bin/interrupts trace.txt vector_table.txt device_table.txt
cat execution.txt
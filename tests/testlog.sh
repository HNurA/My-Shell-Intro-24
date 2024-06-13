#!/bin/bash
echo "Running test: with the given input..."
echo
make all

expected=$(printf "ls\n-al\ntestlog.sh\nexit")

./myshell  > logtest.txt &
out=$?
pid=$!

echo "ls" > /proc/$pid/fd/0
echo "ls -al" > /proc/$pid/fd/0
echo "more testlog.sh" > /proc/$pid/fd/0
echo "exit" > /proc/$pid/fd/0
sleep 1


if [[ $out == 0 ]] ; then
  echo "Program exited zero"
else
  echo "Fail: Program did not exit zero"
  exit 1
fi

actual=$(cat log.txt | awk '{print $NF}')

pid=$(pgrep myshell)
if ! $pid; then
  echo "3"
  kill -9 $pid  
fi

if [[ "$actual" =~ "$expected" ]] ; then
  echo "Pass: Output is correct"
  echo "$expected ->> $actual"
else
  echo "Fail: Expected '$expected' but got: $actual"
  exit 1
fi

echo
echo "test passed!"
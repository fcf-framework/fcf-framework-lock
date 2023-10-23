#!/bin/bash
while true
do
  #npm install
  node ./tests/tests.js
  if [[ $? != 0 ]]; then
    echo "Test error!"
    exit 1;
  fi
done

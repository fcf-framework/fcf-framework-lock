#!/bin/bash
NODES=~/applications/node
selectors=()
for entry in `ls $NODES`; do
  if [[ $entry == "select-node"* ]]; then
    number=`echo ${entry} | sed 's/select-node-v\([0-9]*\).*/\1/'`
    number=`printf "%03d" ${number}`
    selectors+=(${number}$entry)
  fi
done

selectors=(`echo ${selectors[@]} | tr " " "\n" | sort`)
newSelectors=()

for value in "${selectors[@]}"
do
  value=${value:3}
  newSelectors+=(${value})
done

for value in "${newSelectors[@]}"
do
  echo "============================"
  echo "Start for "${value}
  source ${NODES}/${value}
  npm install
  if [[ $? != 0 ]]; then
    echo "Build error!"
    exit 1;
  fi
  node ./tests/tests.js
  if [[ $? != 0 ]]; then
    echo "Test error!"
    exit 1;
  fi
done

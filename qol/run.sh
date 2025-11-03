#!/bin/bash

./qol/test.sh
printf '\nHere are the contents of thoughts.typ\n\"\n'
cat Diary/10.October/thoughts:2.typ
printf '\"\n'
rm main

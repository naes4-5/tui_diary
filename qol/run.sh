#!/bin/bash

bash qol/cleanup.sh 
printf '\nHere are the contents of thoughts.md\n\"\n'
cat Diary/10.October/thoughts.md
printf '\"\n'
bash qol/test.sh

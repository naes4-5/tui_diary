#!/bin/bash

bash qol/test.sh
printf '\nHere are the contents of thoughts.md\n\"\n'
cat Diary/10.October/thoughts.md
printf '\"\n'
rm -rf Diary/

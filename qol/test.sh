#!/bin/bash

gcc main.c -o main -fsanitize=address && ./main && rm main

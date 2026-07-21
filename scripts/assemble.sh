#!/bin/bash

gcc -S -O -fno-asynchronous-unwind-tables -fcf-protection=none $1

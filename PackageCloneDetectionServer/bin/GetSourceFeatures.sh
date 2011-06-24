#!/bin/bash

grep '\.[hc] ' $1 |sed -e 's/\(.*\) \(.*\) \(.*\)/\1 \2/g'

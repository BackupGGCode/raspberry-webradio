#!/bin/bash

pngtopnm $1 > tmp
pnmtopgm tmp | pgmtopbm | pbmtoxbm > img.h
rm tmp

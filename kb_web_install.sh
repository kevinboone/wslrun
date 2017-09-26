#!/bin/bash
WEB=/home/kevin/docs/kzone5/
SOURCE=$WEB/source
TARGET=$WEB/target
make clean
cp README_wslrun.html $SOURCE
(cd $WEB; ./make.pl wslrun)

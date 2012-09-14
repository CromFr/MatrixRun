#!/bin/sh
#
#	Custom shell script
#

cd MatrixRun
valgrind  --trace-children=yes --leak-check=full --log-file=../valgrind/out.log --xml=yes --xml-file=../valgrind/out.xml ./bin/linux/Debug/MatrixRun
cd ../
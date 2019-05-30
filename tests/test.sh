#!/bin/bash
cd ..
make
cd tests
rm -f shrunk.dat
rm -f unshrunk.dat
TESTCASE=$1
../shrinker shrink $TESTCASE shrunk.dat
../shrinker unshrink shrunk.dat unshrunk.dat
echo 'Input:'
xxd $TESTCASE
echo 'Shrunk:'
xxd shrunk.dat
echo 'Unshrunk:'
xxd unshrunk.dat
diff $TESTCASE unshrunk.dat

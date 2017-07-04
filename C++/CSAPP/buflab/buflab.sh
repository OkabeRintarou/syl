#!/bin/bash

USERID=OkabeRintarou
make

echo "level 0:"
./hex2raw < level0.txt  | ./bufbomb -u $USERID
echo "level 1:"
./hex2raw < level1.txt  | ./bufbomb -u $USERID
echo "level 2:"
./hex2raw < level2.txt  | ./bufbomb -u $USERID
echo "level 3:"
./hex2raw < level3.txt  | ./bufbomb -u $USERID
echo "level 4:"
./hex2raw -5 < level4.txt  | ./bufbomb -n -u $USERID

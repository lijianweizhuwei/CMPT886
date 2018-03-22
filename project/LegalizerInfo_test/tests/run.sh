#!/usr/bin/env bash

echo "### Running original version...  ###"
llc -filetype=asm $1.ll && clang -o $1.bin $1.s && ./$1.bin

echo ""
echo "### Running legalized version... ###"
if [ -d ../xcode ]; then
    opt -S -load ../xcode/BitcastLegalization/Debug/libBitcastLegalization.so -bitcastlegalization < $1.ll > $1-legalized.ll
else
    opt -S -load ../build/BitcastLegalization/libBitcastLegalization.so -bitcastlegalization < $1.ll > $1-legalized.ll
fi
llc -filetype=asm $1-legalized.ll && clang -o $1-legalized.bin $1-legalized.s && ./$1-legalized.bin

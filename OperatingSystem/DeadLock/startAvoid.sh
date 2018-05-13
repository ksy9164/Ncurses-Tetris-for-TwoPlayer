#! /bin/bash
gcc -w avoidPA.c -o apa.ksy
gcc -w avoidPB.c -o apb.ksy
gcc -w avoidPC.c -o apc.ksy
./apa.ksy & ./apb.ksy & ./apc.ksy
rm *.out
ipcrm -a

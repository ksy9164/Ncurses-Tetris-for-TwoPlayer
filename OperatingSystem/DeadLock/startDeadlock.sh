#! /bin/bash
gcc -w deadlockPA.c -o dpa.ksy
gcc -w deadlockPB.c -o dpb.ksy
gcc -w deadlockPC.c -o dpc.ksy
./dpa.ksy & ./dpb.ksy & ./dpc.ksy
rm *.ksy
ipcrm -a

#! /bin/bash
gcc -w preventionPA.c -o ppa.ksy
gcc -w preventionPB.c -o ppb.ksy
gcc -w preventionPC.c -o ppc.ksy
./ppa.ksy & ./ppb.ksy & ./ppc.ksy
rm *.ksy
ipcrm -a

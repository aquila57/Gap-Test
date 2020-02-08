#!/bin/bash
make -f libgap.mak clean
make -f gaptest.mak clean
make -f etausgap.mak clean
make -f lfsrgap.mak clean
make -f sinegap.mak clean
make -f fibogap.mak clean
make -f rugap.mak clean
make -f gslgap.mak clean
make -f diegap.mak clean
rm -f allgsl.out alldie.out

#!/bin/sh

export LD_LIBRARY_PATH=lib
export LD_PRELOAD=preload/libpandorahck.so 

./spacerocks

#!/bin/bash

mkdir -p _build
cd _build
cmake .. -DCMAKE_INSTALL_PREFIX=. -DSERIOUS_PROTON_DIR=../../SeriousProton/ -DENABLE_CRASH_LOGGER=ON
make \
    && make install \
    && EmptyEpsilon.app/Contents/MacOS/EmptyEpsilon

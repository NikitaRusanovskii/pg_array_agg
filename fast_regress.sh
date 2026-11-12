#!/bin/bash
clear
make USE_PGXS=1 clean
make USE_PGXS=1
make USE_PGXS=1 install
make USE_PGXS=1 installcheck
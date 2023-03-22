#! /bin/bash

cmake .
make 
cp dtree /usr/bin
echo "Succesfully installed. Path: /usr/bin/ds"
#!/usr/bin/env bash

#===============================================================================
#
#          FILE: travis_build_linux.sh
#
#         USAGE: ./travis_build_linux.sh
#
#   DESCRIPTION:  Build  on linux environment.
#
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: Dilawar Singh (), dilawars@ncbs.res.in
#  ORGANIZATION: NCBS Bangalore
#       CREATED: 01/02/2017 10:11:46 AM
#      REVISION:  ---
#===============================================================================

set -e
set -x

PYTHON2="/usr/bin/python2"
PYTHON3="/usr/bin/python3"
MAKEFLAGS="-j`nproc`"

if [ ! -n "$MAKE" ]; then
    MAKE="make -j`nproc`"
else
    MAKE="$MAKE -j`nproc`"
fi

unset PYTHONPATH

# Bug: `which python` returns /opt/bin/python* etc on travis. For which numpy
# many not be available. Therefore, it is neccessary to use fixed path for
# python executable.

mkdir -p _BUILD && cd _BUILD
cmake ..
$MAKE && ctest --output-on-failure 

#!/bin/bash

# Color
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

./runServer.sh --directory /tmp

# Request
echo "${pwd}"
./test01.sh
#!/bin/bash

# Color
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

# pre run hook
echo -e "Server: ${GREEN}$(pwd)/src/main.cpp${NC}"
echo -e "Client: ${GREEN}$(pwd)/tesh01.sh${NC}"
echo "-----------------------------------------------------"
cat $(pwd)/.githooks/aman.txt
echo -e "Welcome to ${BLUE}amanRaj7${NC}! Code execution started."
echo "-----------------------------------------------------"

./test.sh
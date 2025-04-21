#!/bin/bash

# Color
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

# This script compiles and runs a C++ program.
SRC_FILE="src/main.cpp"
OUT_FILE="server"
g++ -std=c++17 "$SRC_FILE" -o "$OUT_FILE" # compile

# Run
if [ $? -ne 0 ]; then
  echo -e  "${RED} Compilation failed!${NC}"
  exit 1
fi
echo -e "${GREEN}Compilation succeeded!${NC}"
echo -e "${BLUE}Starting server on port 4221...${NC}"
./"$OUT_FILE" &
SERVER_PID=$!
echo -e "${BLUE}Server PID: $SERVER_PID${NC}"

# Wait for server to start
sleep 2

# Request
echo "${pwd}"
./test01.sh

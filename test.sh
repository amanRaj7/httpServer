#!/bin/bash

# Color
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

# Start the server in the background
echo -e "${BLUE}Starting server...${NC}"
./runServer.sh --directory /tmp

# Run requests
echo -e "${BLUE}Running test requests...${NC}"
./test01.sh
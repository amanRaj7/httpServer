#!/bin/bash

#color
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

# Send request
echo -e "${BLUE}\nSending test requests:\n${BLUE}"

URLS=(
 "curl -v --header "User-Agent: foobar/1.2.3" http://localhost:4221/user-agent", "http://localhost:4221/echo/aman"
)

for url in "${URLS[@]}"; do
  echo "➡️ Request: $url"
  echo -e ${GREEN}
  curl -i "$url"
  echo -e ${NC}
  echo -e "\n-----------------------------\n"
done

echo -e "${GREEN}Done!${NC}"

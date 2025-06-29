#!/bin/bash

# color
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

# Simulate raw HTTP GET requests concurrently
echo -e "${BLUE}\nLaunching background netcat (nc) requests:\n${NC}"

(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &

# Send curl requests
echo -e "${BLUE}\nSending test requests:\n${BLUE}"

URLS=(
  "curl -v -i --header 'User-Agent: foobar/1.2.3' http://localhost:4221/user-agent"
  "curl -v -i http://localhost:4221/echo/aman"
)

for url in "${URLS[@]}"; do
  echo -e "${BLUE}\n➡️ Request: $url${NC}"
  echo -e "${GREEN}"
  eval "$url"
  echo -e "${NC}"
  echo -e "\n-----------------------------\n"
done

echo -e "${GREEN}Done!${NC}"

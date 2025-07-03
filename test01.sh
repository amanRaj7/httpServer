#!/bin/bash

# color
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

# Prepare test file
echo -e "${BLUE}\nPrepare test file${NC}"
echo -n 'Hello, World!' > /tmp/foo

# Simulate raw HTTP GET requests concurrently
echo -e "${BLUE}\nLaunching background netcat (nc) requests:\n${NC}"

(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
(sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &

# Send curl requests
echo -e "${BLUE}\nSending test requests:\n${BLUE}"

URLS=(
  "curl -i --header 'User-Agent: foobar/1.2.3' http://localhost:4221/user-agent"
  "curl -i http://localhost:4221/echo/aman"
  "curl -i http://localhost:4221/files/foo"
  "curl -v --data '12345' -H 'Content-Type: application/octet-stream' http://localhost:4221/files/file_123"
)

for url in "${URLS[@]}"; do
  echo -e "${BLUE}\n➡️ Request: $url${NC}"
  echo -e "${GREEN}"
  eval "$url"
  echo -e "${NC}"
  echo -e "\n-----------------------------\n"
done

echo -e "${GREEN}Done!${NC}"

#!/bin/bash
git config core.hooksPath .githooks
chmod +x ./.githooks/pre-push
chmod +x ./test01.sh
chmod +x ./runServer.sh
chmod +x ./run.sh
chmod +x ./test.sh
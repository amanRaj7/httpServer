#!/bin/bash
git config core.hooksPath .githooks

# This script compiles and runs a C++ program.
# SRC_FILE="src/main.cpp"
# OUT_FILE="main"
# g++ -std=c++17 "$SRC_FILE" -o "$OUT_FILE" # compile
# # Run
# if [ $? -eq 0 ]; then
#     echo "✅ Compilation successful. Running program..."
#     echo "-----------------------------"
#     ./"$OUT_FILE"
#     echo "-----------------------------"
# else
#     echo "❌ Compilation failed."
# fi
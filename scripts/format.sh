#!/bin/bash

printf "Formatting code...\n"
find src/ -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=file
if [ $? -ne 0 ]; then
    printf "Failed to format source code\n"
    exit 1
fi

printf "Done.\n"

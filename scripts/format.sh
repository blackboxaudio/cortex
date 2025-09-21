#!/bin/bash

printf "Formatting code...\n"

find include/ -iname '*.h' | xargs clang-format -i -style=file
if [ $? -ne 0 ]; then
    printf "Failed to format header files\n"
    exit 1
fi

find src/ -iname '*.cpp' -o -iname '*.h' | xargs clang-format -i -style=file
if [ $? -ne 0 ]; then
    printf "Failed to format source files\n"
    exit 1
fi

printf "Done.\n"

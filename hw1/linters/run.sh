#!/usr/bin/env bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    #if [ $STATUS -ne 0 ]
    #then
        #exit $STATUS
    #fi
}

print_header "RUN cppcheck"
check_log "cppcheck hw1 --enable=all --inconclusive --error-exitcode=1 -I hw1/weather/include --suppress=missingIncludeSystem -i hw1/build/" "\(information\)"

print_header "RUN clang-tidy"
check_log "clang-tidy hw1/weather/src/* hw1/weather/include/* -warnings-as-errors=* -extra-arg=-std=c99 -- -Ihw1/weather/include" "Error (?:reading|while processing)"

print_header "RUN cpplint"
check_log "cpplint --extensions=c hw1/weather/include/* hw1/weather/src/*" "Can't open for reading"

print_header "SUCCESS"
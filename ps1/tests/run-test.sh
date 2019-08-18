#! /bin/sh

# ps1-4git - A git-aware section for your PS1 prompt
# Copyright (c) 2019, Yassen Damyanov
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the MIT License.
# You should have received a copy of the MIT License along with this program.
# If not, see <https://opensource.org/licenses/MIT>.

# Run test-ps1-4git and hide output on success; show on failed tests.
# Assuming execution is done from parent directory ps1/.

_out=$(./tests/test-ps1-4git 2>&1)  #  && _ok=yes || _ok=no
_tests_failed=$(echo "$_out" | awk '/[ ]+tests[ ]+/ {print $5}')

if [ x"$_tests_failed" = x0 ]; then
    echo "\nAll tests passed - OK"
else
    msg="***** $_tests_failed test(s) FAILED!"
    echo "\n\n$msg$_out\n\n$msg"
fi

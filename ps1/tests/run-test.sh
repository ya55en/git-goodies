#! /bin/sh
#
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

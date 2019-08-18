#! /bin/sh

# ps1-4git - A git-aware section for your PS1 prompt
# Copyright (c) 2019, Yassen Damyanov
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the MIT License.
# You should have received a copy of the MIT License along with this program.
# If not, see <https://opensource.org/licenses/MIT>.
#
# Print home root directory for this system to stdout.

useradd_conf=/etc/default/useradd

_script=$(cat <<EOS
  BEGIN { found = 0 }
  /^\s*HOME\s*=/ { found = 1; print \$2 }
  END { if (!found) print "/home"}
EOS
)

result=$(awk -F= "$_script" $useradd_conf)

echo "$result" | sed 's:^\s*\(.*\)\s*$:\1:'

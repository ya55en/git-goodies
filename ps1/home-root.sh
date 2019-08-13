#! /bin/sh

useradd_conf=/etc/default/useradd

_script=$(cat <<EOS
  BEGIN { found = 0 }
  /^\s*HOME\s*=/ { found = 1; print \$2 }
  END { if (!found) print "/home"}
EOS
)

result=$(awk -F= "$_script" $useradd_conf)

echo "$result" | sed 's:^\s*\(.*\)\s*$:\1:'


#!/bin/sh

system_code="10111"

unit_code="01000" #A, B, C, D, E switch

switch=1

elroframe="$system_code$unit_code"

elroframe=$( echo $elroframe | sed 's|1|136 |g' )
elroframe=$( echo $elroframe | sed 's|0|142 |g' )

if [ $switch -gt 0 ]; then
  elroframe="${elroframe}136 142 "
else
  elroframe="${elroframe}142 136 "
fi

elroframe="${elroframe}128 0 0 0"

export IFS=" "

buffer=""

for framepart in $elroframe; do
  framepart=$(printf "%0*d\n" 8 $(echo "obase=2;$framepart" | bc))
  buffer="$buffer$framepart"
done

elroframe=$buffer

echo $elroframe > /tmp/transmitterfifo

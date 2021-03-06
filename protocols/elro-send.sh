#!/bin/sh

system_code="10111"

unit_code="01000" #A, B, C, D, E switch

if [ "$1" = "E" ] ; then
  unit_code="00001"
elif [ "$1" = "D" ] ; then
  unit_code="00010"
elif [ "$1" = "C" ] ; then
  unit_code="00100"
elif [ "$1" = "B" ] ; then
  unit_code="01000"
elif [ "$1" = "A" ] ; then
  unit_code="10000"
fi

switch=1

if [ "$2" = "1" ] ; then
  switch=1
elif [ "$2" = "0" ] ; then
  switch=0
fi

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

#make frame 1.5s long
a=0
while [ $a -lt 90 ]
do
  elroframe="$elroframe$buffer"
  a=`expr $a + 1`
done

echo $elroframe > /tmp/transmitterfifo

#!/bin/bash
while [ 1 ]
do
read
if [ $REPLY = "zzz" ]
then
exit 0
fi
gslgap.sh $REPLY
done

#!/bin/bash

cd /home/pi

# stop any running stream
if [ -f stream.pid ];
then
 ./stop_stream.sh
fi

./usb_play "$1" &
PID=$!
echo $PID > stream.pid

exit


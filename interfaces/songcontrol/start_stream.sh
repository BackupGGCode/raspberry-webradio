#!/bin/bash

cd /home/pi

pc=`pgrep start_stream | wc -l | cut -d " " -f1`

# can only start if we are the only instance of this script
if [ $pc -gt 2 ]; then
  echo "Another stream is starting, aborting..."
  exit;
fi

sc=`pgrep -x stream | wc -l | cut -d " " -f1`

# can only start if no ore one stream playing
if [ $sc -lt 2 ]; then

  # stop any running stream
  if [ -f stream.pid ];
  then
  ./stop_stream.sh
  fi

  ./stream $1 &
  PID=$!
  echo $PID > stream.pid
  exit

else
  echo "Already $sc  streams running, aborting..."
fi

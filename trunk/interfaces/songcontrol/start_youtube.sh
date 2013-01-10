#!/bin/bash

cd /home/pi

pc=`pgrep start_youtube | wc -l | cut -d " " -f1`

# can only start if we are the only instance of this script
if [ $pc -gt 2 ]; then
  echo "Another stream is starting, aborting..."
  sudo pkill youtube-dl
  sudo pkill omxplayer
fi

sc=`pgrep -x stream | wc -l | cut -d " " -f1`

# can only start if not more than one stream playing
if [ $sc -lt 2 ]; then

  # stop any running stream
  if [ -f stream.pid ];
  then
  ./stop_stream.sh
  fi

  omxplayer -o local `youtube-dl -g "$1"`
  exit

else
  echo "Already $sc  streams running, aborting..."
fi

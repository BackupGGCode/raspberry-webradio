#!/bin/bash

cd /home/pi

PID=`cat stream.pid`
kill -2 $PID
rm stream.pid

sudo pkill youtube-dl
sudo pkill omxplayer
exit
 

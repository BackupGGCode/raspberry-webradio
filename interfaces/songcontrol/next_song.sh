#!/bin/bash

cd /home/pi

PID=`cat stream.pid`
kill -1 $PID
exit
 

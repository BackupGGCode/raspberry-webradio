#!/bin/bash
 
sudo iwlist wlan0 scanning | grep ESSID | tr -d ' "' | sed -e 's/ESSID://' > /home/pi/wifi.txt

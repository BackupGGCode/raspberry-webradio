#!/bin/bash

echo "+--------------------------------------------------------------+"
echo "|                                                              |"
echo "|                WebRadio Installer v1.0                       |"
echo "|                                                              |"
echo "|  Copyright (C) 2012 by Michael Schwarz                       |"
echo "|                                                              |"
echo "+--------------------------------------------------------------+"

# user must be root
if [[ ${EUID} -ne 0 ]]; then
	echo "This script must be run as root"
	exit 1
fi

# install required software
if [ ! -f /usr/sbin/avahi-daemon ];
then
  echo
  echo "Installing avahi-daemon to access webradio as \"raspberrypi.local\""
  echo
  apt-get install avahi-daemon
fi

if [ ! -f /usr/bin/mpg123 ];
then
  echo
  echo "Installing mpg123"
  echo
  apt-get install mpg123
fi

if [ ! -f /usr/bin/id3v2 ];
then
  echo
  echo "Installing id3v2"
  echo
  apt-get install id3v2
fi

if [ ! -f /usr/share/usbmount/usbmount ];
then
  echo 
  echo "Installing usbmount"
  echo
  apt-get install usbmount
fi

if [ ! -f /usr/sbin/lighttpd ];
then
  echo
  echo "Installing lighthttpd + php5"
  echo
  apt-get install lighttpd php5-cgi php5-mysql php5-curl php5-gd  php5-cgi php5-cli php5-common 
  echo
  echo "Create webserver directory"
  echo
  mkdir /var/www 2> /dev/null
  chown pi:pi /var/www
  rm /var/log/lighttpd/error.log
fi

if [ ! -f /usr/local/lib/libwiringPi.a ];
then
  echo
  echo "Installing wiringPi"
  echo
  cd tools/wiringPi/wiringPi
  make clean 2>&1 > /dev/null
  make 2>&1 > /dev/null
  make install
  cd ../../..
fi

if [ ! -f /usr/include/curl/curl.h ];
then
  echo
  echo "Installing libcurl"
  echo
  sudo apt-get install libcurl4-openssl-dev
fi

# copy mobile page
echo
echo "Copy mobile page to webserver directory"
echo 
cp -R mobile_page/* /var/www/ 2> /dev/null
chown pi:pi /var/www/*

# setup lighthttpd
echo
echo "Settings up lighttpd"
echo
mv /etc/lighttpd/lighttpd.conf /etc/lighttpd/lighthttpd.conf.old
cp lighttpd.conf /etc/lighttpd/lighttpd.conf
/etc/init.d/lighttpd restart


# delete old firmware
echo 
echo "Delete old firmware"
echo
rm /home/pi/firmware 2> /dev/null

# build songcontrol
echo
echo "Build songcontrol"
echo
cd interfaces/songcontrol
make clean 2>&1 > /dev/null
make 2>&1 > /dev/null
cd ../..

# build firmware
echo
echo "Build firmware"
echo
cd software
make clean 2>&1 > /dev/null
make 2>&1 > /dev/null
cd ..

# copy file to home folder
echo
echo "Install firmware and songcontrol"
echo
cp interfaces/songcontrol/*.sh /home/pi/ 2> /dev/null
cp interfaces/songcontrol/usb_play /home/pi/ 2> /dev/null
cp interfaces/songcontrol/stream /home/pi/ 2> /dev/null
cp interfaces/songcontrol/play.conf /home/pi/ 2> /dev/null
cp software/firmware /home/pi/ 2> /dev/null

# set root rights to firmware
echo
echo "Set rights for firmware"
echo
chown root /home/pi/firmware
chmod 4755 /home/pi/firmware
chmod +x /home/pi/*.sh
chown root /home/pi/wifi_connect.sh
chown root /home/pi/wifi_scan.sh
chmod 4755 /home/pi/wifi_connect.sh
chmod 4755 /home/pi/wifi_scan.sh

# copy language files
echo
echo "Install language files"
echo
mkdir /home/pi/language 2> /dev/null
cp software/language/*.lng /home/pi/language/ 2> /dev/null

# copy default config
echo
echo "Write default configuration file"
echo
cp software/default.conf /home/pi/ 2> /dev/null


# done
echo "----------------------------------------------------------------"
echo "  Installation completed"
echo
echo "  To start, type \"cd /home/pi; ./firmware\""
echo "----------------------------------------------------------------"


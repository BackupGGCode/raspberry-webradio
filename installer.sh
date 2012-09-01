#!/bin/bash

echo "+--------------------------------------------------------------+"
echo "|                                                              |"
echo "|                WebRadio Installer v1.0                       |"
echo "|                                                              |"
echo "|  Copyright (C) 2012 by Michael Schwarz                       |"
echo "|                                                              |"
echo "+--------------------------------------------------------------+"

# delete old installation
sudo rm /home/pi/firmware

# build songcontrol
cd interfaces/songcontrol
make clean
make


# build firmware
cd ../../software
make clean
make

# copy file to home folder
cd ..
cp interfaces/songcontrol/*.sh /home/pi/
cp interfaces/songcontrol/usb_play /home/pi/
cp interfaces/songcontrol/stream /home/pi/
cp software/firmware /home/pi/

# set root rights to firmware
sudo chown root /home/pi/firmware
sudo chmod 4755 /home/pi/firmware

# copy language files
mkdir /home/pi/language
cp software/language/*.lng /home/pi/language/

# copy default config
cp software/default.conf /home/pi/

# done
echo "----------------------------------------------------------------"
echo "  Installation completed"
echo "----------------------------------------------------------------"


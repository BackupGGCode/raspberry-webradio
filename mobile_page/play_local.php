<?php
include("settings.inc.php");
include("station.php");

if(isset($_GET["play"])) {
 $path = $usb_path;
 
 if(isset($_GET["rel"]) && $_GET["rel"] == 1) $path = ""; 
 file_put_contents($current_station_path, "USB");
 shell_exec("/home/pi/start_local.sh \"" . $path . base64_decode($_GET["play"]) . "\" > /dev/null 2>/dev/null &");
 echo "USB";
}

if(isset($_GET["next"])) {
 shell_exec("/home/pi/next_song.sh > /dev/null 2>/dev/null &");
 echo "USB";
}

?>
 

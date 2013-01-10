<?php
include("settings.inc.php");
include("station.php");

if(isset($_GET["play"])) {
 file_put_contents($current_station_path, "Youtube");
 file_put_contents($songfile_path, "N/A - " . $_GET["play"]);
 echo "/home/pi/start_youtube.sh \"" . $_GET["play"] . "\"";
 shell_exec("sudo /home/pi/start_youtube.sh \"" . $_GET["play"] . "\" &");
}

?>
 

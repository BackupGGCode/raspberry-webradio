<?php
include("settings.inc.php");
include("station.php");

if(isset($_GET["play"])) {
 echo get_station_name($_GET["play"]);
 file_put_contents($current_station_path, get_station_name($_GET["play"]));
 shell_exec("/home/pi/start_stream.sh " . get_station_url($_GET["play"]) . " > /dev/null 2>/dev/null &");
}

?>
 

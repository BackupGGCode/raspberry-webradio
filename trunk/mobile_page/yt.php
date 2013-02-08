<?php
include("settings.inc.php");
include("station.php");

if(isset($_GET["play"])) {
 $title = $_GET["play"];
 if(isset($_GET["title"])) $title = $_GET["title"];

 $artist = "N/A";
 if(isset($_GET["artist"])) $artist = $_GET["artist"];

 file_put_contents($current_station_path, "Youtube");
 file_put_contents($songfile_path, $artist . " - " . $title);
 echo "/home/pi/start_youtube.sh \"" . $_GET["play"] . "\"";
 shell_exec("sudo /home/pi/start_youtube.sh \"" . $_GET["play"] . "\" &");
}

?>
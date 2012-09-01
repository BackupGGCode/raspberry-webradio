<?php 
include("settings.inc.php");

if(trim(file_get_contents($current_station_path)) == "USB") {
  echo "1";
} else echo "0";
?>
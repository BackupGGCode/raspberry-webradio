<?php
include("settings.inc.php");

if(file_exists($current_station_path)) {
 echo file_get_contents($current_station_path);
}

?>
 

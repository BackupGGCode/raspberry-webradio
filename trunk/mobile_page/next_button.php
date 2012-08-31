<?php 
if(trim(file_get_contents("current_station.txt")) == "USB") {
  echo "1";
} else echo "0";
?>
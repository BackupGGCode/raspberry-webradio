<?php
include("settings.inc.php");

echo "-";
file_put_contents($current_station_path, "-");
file_put_contents($songfile_path, "(no artist) - (no title)");
shell_exec("/home/pi/stop_stream.sh > /dev/null 2>/dev/null &");

?>
 

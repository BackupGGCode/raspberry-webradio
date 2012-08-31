<?php
include("settings.inc.php");

$song = file_get_contents($songfile_path);
$parts = explode(" - ", $song);
echo trim($parts[1]);

?>
 

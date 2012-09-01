<?php


function get_station_name($id) {
 global $stations_path;
 $stations = file($stations_path);
 $info = explode("|", $stations[$id]);
 return $info[0];
}

function get_station_url($id) {
 global $stations_path;
 $stations = file($stations_path);
 $info = explode("|", $stations[$id]);
 return $info[1];
}

function get_station_genre($id) {
 global $stations_path;
 $stations = file($stations_path);
 $info = explode("|", $stations[$id]);
 return $info[2];
}

function get_stations() {
 global $stations_path;
 $stations = file($stations_path);
 return count($stations);
}

function get_current_station() {
 global $current_station_path;
 return file_get_contents($current_station_path);
}

function add_station($name, $url, $genre) {
 global $stations_path;
 $s = file($stations_path);
 $s[] = $name . "|" . $url . "|" . $genre . "|0\r\n";
 file_put_contents($stations_path, $s);
}

function delete_station($id) {
 global $stations_path;
 $stations = file($stations_path);

 $new_station = "";
 for($i = 0; $i < count($stations); $i++) {
  if($i != $id) $new_station .= trim($stations[$i]) . "\r\n";
 }

 file_put_contents($stations_path, $new_station);
}


?>

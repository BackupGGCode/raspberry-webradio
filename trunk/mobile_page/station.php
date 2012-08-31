<?php


function get_station_name($id) {
 $stations = file("stations.txt");
 $info = explode("|", $stations[$id]);
 return $info[0];
}

function get_station_url($id) {
 $stations = file("stations.txt");
 $info = explode("|", $stations[$id]);
 return $info[1];
}

function get_station_genre($id) {
 $stations = file("stations.txt");
 $info = explode("|", $stations[$id]);
 return $info[2];
}

function get_stations() {
 $stations = file("stations.txt");
 return count($stations);
}

function get_current_station() {
 return file_get_contents("current_station.txt");
}

function add_station($name, $url, $genre) {
 $s = file("stations.txt");
 $s[] = $name . "|" . $url . "|" . $genre . "|0\r\n";
 file_put_contents("stations.txt", $s);
}

function delete_station($id) {
 $stations = file("stations.txt");

 $new_station = "";
 for($i = 0; $i < count($stations); $i++) {
  if($i != $id) $new_station .= trim($stations[$i]) . "\r\n";
 }

 file_put_contents("stations.txt", $new_station);
}


?>

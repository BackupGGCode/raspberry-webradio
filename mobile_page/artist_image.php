<?php
include("settings.inc.php");

$song = file_get_contents($songfile_path);
$parts = explode(" - ", $song);
$name = trim($parts[0]);

$current_name = trim(file_get_contents("current_artist.txt"));
// we already have the image
if($current_name == $name) {
  $img = file_get_contents("current_artist_img.txt");
} else {
  $url = "http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&artist=" . urlencode($name) . "&api_key=" . $last_fm_key;
  $info = file($url);

  $img = "";
  for($i = 0; $i < count($info); $i++) {
    $info[$i] = trim($info[$i]);
    if(substr($info[$i], 0, 20) == "<image size=\"large\">") {
      $img = substr($info[$i], 20, strlen($info[$i]) - 28);
      break;
    }
  }

  file_put_contents("current_artist.txt", $name);
  file_put_contents("current_artist_img.txt", $img);
}

if($img != "") {
 echo "<img src=\"$img\" style=\"border: 1px solid gray; max-height:128px;\"/>";
} else {
 echo "<img src=\"music_playing.png\" style=\"border: 1px solid gray; max-height:128px;\"/>";
}


?>
 

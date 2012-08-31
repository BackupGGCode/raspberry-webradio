<?php
include("settings.inc.php");

function get_usb_path() {
 return "/media/usb/";
}

function usb_listview() {
 $path = get_usb_path(); //"/media/usb/";
 $folder = "";

 if(isset($_GET["root"])) {
  $path .= $_GET["root"] . "/";
  $folder .= $_GET["root"] . "/";
 }

 
 $p = explode("/", $folder);
  
 if(count($p) >= 2) {
   unset($p[count($p) - 1]);
   unset($p[count($p) - 1]);
   $up = implode("/", $p);
 } else {
   $up = $folder;
 }
 

 $listview = "";
 $dir = opendir($path);

 $folders = array();
 $songs = array();

 // read files and folders
 while($file = readdir($dir)) {
   if($file != "." && $file != "..") {
     // a file
     if(filetype($path . $file) == "file") {
       // if it ends with mp3, add to list
       if(substr($file, strlen($file) - 4, 4) == ".mp3") $songs[] = $file;
     } else {
       // a folder
       $folders[] = $file;
     }
   }
 }
 closedir($dir);
  
 // add folder up to listview if not at root directory 
 if(isset($_GET["root"]) && strlen($_GET["root"]) != 0) { 
   $listview .= "<li><a href=\"?root=" . urlencode($up) . "#usb\" rel=\"external\" data-icon=\"arrow-u\"><img src=\"arrow_up.png\" class=\"ui-li-icon\" /> ..</a></li>\r\n";
 }
 // add folders to listview
 for($i = 0; $i < count($folders); $i++) {
   $listview .= "<li><a href=\"?root=" . urlencode($folder . $folders[$i]) . "#usb\" rel=\"external\"><img src=\"folder.gif\" class=\"ui-li-icon\" /> ". $folders[$i] . "</a></li>\r\n";
 }
 // add songs to listview
 for($i = 0; $i < count($songs);$i++) {
   $listview .= "<li><a href=\"?play_local=" . urlencode($folder . $songs[$i]) . "#page1\" rel=\"external\"><img src=\"mp3.gif\" class=\"ui-li-icon\" /> " . $songs[$i] . "</a></li>\r\n";
 }
 

 return $listview;
}



function generate_playlist($folder) {
 if($folder[strlen($folder) - 1] != "/") $folder .= "/";
 $path = get_usb_path();
 $dir = opendir($path . $folder);
 $pls = "";

 // read files and folders
 while($file = readdir($dir)) {
   if(filetype($path . $folder . $file) == "file") {
     // if it ends with mp3, add to list
     if(substr($file, strlen($file) - 4, 4) == ".mp3") $pls .= $path . $folder . $file . "\r\n";
   }
 }
 closedir($dir);
 file_put_contents("/home/pi/playlist.m3u", $pls);
}


?> 

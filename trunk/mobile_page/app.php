<?php
include("settings.inc.php");
include("station.php");
include("usb.php");

$song = file_get_contents($songfile_path);
$parts = explode("-", $song);

// save new station
if(isset($_GET["url"])) {
  add_station($_GET["name"], $_GET["url"], $_GET["genre"]);
}

// parse shoutcast station
if(isset($_GET["sc_url"])) {
 $info = file($_GET["sc_url"]);
 for($i = 0; $i < count($info); $i++) {
   if(substr($info[$i], 0, 7) == "Title1=") $title = trim(substr($info[$i], 7));
   if(substr($info[$i], 0, 6) == "File1=") $url = trim(substr($info[$i], 6));
 }

 // remove (#1 - xxxx/xxxx) at the beginnning of the name
 if(substr($title, 0, 2) == "(#") {
  while(substr($title, 0, 1) != ")") $title = substr($title, 1);
  $title = substr($title, 1);
 }
 
 if(isset($_GET["sc_genre"]) && strlen(trim($_GET["sc_genre"])) > 1) $genre = $_GET["sc_genre"];
 if(isset($_GET["sc_name"]) && strlen(trim($_GET["sc_name"])) > 1) $title = $_GET["sc_name"];
 
 $title = trim($title);
 $genre = trim($genre);
 
 add_station($title, $url, $genre);
}

// delete station
if(isset($_GET["del_item_id"])) {
 $id = $_GET["del_item_id"];
 delete_station($id);
 header("Location:app.php#page2");
}

// play folder
if(isset($_GET["play_folder"])) {
 $folder = base64_decode($_GET["play_folder"]);
 generate_playlist($folder);
 $_GET["play_local"] = "playlist.m3u";
 $_GET["play_local_rel"] = 1;
}
?>

<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <title>
        </title>
        <link rel="stylesheet" href="jquery.mobile-1.1.0.min.css" />
        <link rel="stylesheet" href="my.css" />
        <style>
            /* App custom styles */
        </style>
        <script src="jquery.min.js">
        </script>
        <script src="jquery.mobile-1.1.0.min.js">
        </script>
        <script src="my.js">
        </script>

<script>
 $(document).ready(function() {

if($('.ui-page-active').attr('id') == "page1") {
   // disable cache
   $.ajaxSetup({ cache: false });

   // load song title
   $("#song_title").load("song.php");
   // refresh every 3 seconds
   setInterval(function() {
      $("#song_title").load('song.php?randval='+ Math.random());
   }, 3000);

   $("#song_artist").load("artist.php");
   var refreshId = setInterval(function() {
      $("#song_artist").load('artist.php?randval='+ Math.random());
   }, 3000);

<?
  if(isset($_GET["play"])) {
   echo "$(\"#song_station\").load(\"play.php?play=" . $_GET["play"] . "\");\r\n";
  } else if(isset($_GET["stop"])) {
   echo "$(\"#song_station\").load(\"stop.php\");\r\n";
  } else if(isset($_GET["play_local"])) {
   echo "$(\"#song_station\").load(\"play_local.php?play=" . base64_encode($_GET["play_local"]) . (isset($_GET["play_local_rel"]) ? "&rel=1" : "") . "\");\r\n";
  } else if(isset($_GET["next"])) {
   echo "$(\"#song_station\").load(\"play_local.php?next=1&rand=\" + Math.random());\r\n";
  } else {
   echo "$(\"#song_station\").load(\"cstation.php\");\r\n";
  }
  
?>

 setInterval(function() {
    $("#song_station").load('cstation.php?randval='+ Math.random());
  }, 3000);
 $("#artist_img").load('artist_image.php?randval='+ Math.random());
 setInterval(function() {
    $("#artist_img").load('artist_image.php?randval='+ Math.random());
  }, 3000);

  $("#next_button").hide();
  var refreshId = setInterval(function() {
   $.post("next_button.php", null, function(data) {
    if(data.indexOf("1") != -1) $("#next_button").show();
    else $("#next_button").hide();
  });
   }, 1000);    

}

  var refreshId = setInterval(function() {
    $("#current_station").load('cstation.php?randval='+ Math.random());
  }, 3000);


});
</script>

    </head>
    <body>

       <!-- =========== STATIONS ============= //-->

        <div data-role="page" id="page2">
            <div data-theme="d" data-role="header">
                <h3>
                    Stations
                </h3>
            </div>
            <div data-role="content" style="padding: 15px">
                <ul data-role="listview" data-divider-theme="d" data-inset="false"  data-filter="true">
                    <li data-theme="d">
                        <a href="?rand=<? echo rand(0,9999); ?>#page1" data-transition="slide" rel="external">
                            <h3>Now playing</h3><p id="current_station">
<?php
echo get_current_station();
?>
</p>
                        </a>
                    </li>
		    <li>
			<a href="?rand=<? echo rand(0, 9999); ?>#usb" data-transition="slide" rel="external">USB</a>
		    </li>
                    <li data-theme="d" data-role="list-divider">
		      Stations                    
                    </li>
<?php

$cnt = get_stations();
for($i = 0; $i < $cnt; $i++) {
 echo "<li data-theme=\"d\">" .
      " <a href=\"?play=$i#page1\" data-transition=\"slide\" rel=\"external\">" .
      get_station_name($i) . 
      "<span class=\"ui-li-count\">" . 
      get_station_genre($i) .
      "</span></a></a><a href=\"#really_delete\" onclick=\"document.getElementById('del_item').value=$i;\" data-rel=\"dialog\" data-position-to=\"window\" data-transition=\"pop\" data-icon=\"delete\">Delete</a></li>";
}

?>
                </ul>
            </div>
            <div data-role="footer" class="ui-bar" data-position="fixed">
	      <a href="#add_station" data-rel="dialog" data-role="button" data-icon="plus">Add</a>
	      <a href="#add_url" data-rel="dialog" data-role="button" data-icon="plus">Add shoutcast URL</a>
            </div>
	  <input type="hidden" value="0" id="del_item" />
        </div>


       <!-- =================== NOW PLAYING ================ -->
       
        <div data-role="page" id="page1">
            <div data-theme="d" data-role="header">
		<a href="#page2" data-role="button" data-icon="arrow-l">Stations</a>
                <h3>
                    Now Playing
                </h3>
            </div>
            <div data-role="content" style="padding: 15px">
                <div style="width: 120px; height: 120px; position: relative;" id="artist_img">
                    <!-- <img src="http://codiqa.com/static/images/v2/image.png" alt="image" style="position: absolute; top: 50%; left: 50%; margin-left: -16px; margin-top: -18px" /> -->
                </div>
                <h2 id="song_title">
                    (no title)
                </h2>
                <h3 id="song_artist">
                    (no artist)
                </h3>
                <h3 id="song_station">
                    -
                </h3>

<center>
<div data-role="controlgroup" data-type="horizontal">
<a href="?stop=1#page1" data-role="button" data-icon="delete" rel="external">Stop</a>
<a href="?next=1&amp;rand=<? echo rand(0,9999); ?>#page1" data-role="button" data-icon="arrow-r" rel="external" id="next_button">Next</a>




</center>
</div>
            </div>
        </div>


       <!-- =================== USB ================ -->
       
        <div data-role="page" id="usb">
            <div data-theme="d" data-role="header">
	    <a href="#page2" data-icon="arrow-l">Stations</a>
                <h3>
                    USB
                </h3>
            </div>
            <div data-role="content" style="padding: 15px">
	      <ul data-role="listview" data-divider-theme="d" data-inset="false"  data-filter="true">
<?
  echo usb_listview();
?>
	      </ul>
	    </div>
            <div data-role="footer" class="ui-bar" data-position="fixed">
	      <a href="#" data-rel="dialog" data-role="button" data-icon="arrow-r">Play all</a>
	      <a href="#" onclick="window.location.href = 'app.php?play_folder=<? echo base64_encode($_GET["root"]); ?>#page1';" data-rel="dialog" data-role="button" data-icon="arrow-r">Play folder</a>
            </div>
	</div>

        
        <!-- =================== ADD STATION ================ -->
        
        <div data-role="page" id="add_station">
         <div data-role="header">
          <h3>Add station</h3>
         </div>
         <div data-role="content" style="padding: 15px">
          <form action="app.php#page2" data-ajax="false" method="GET">
	   <div data-role="fieldcontain">
	    <label for="name">Name:</label>
	    <input type="text" name="name" id="name" value="">

	    <label for="url">URL:</label>
	    <input type="text" name="url" id="url" value="">

    	    <label for="genre">Genre:</label>
	    <input type="text" name="genre" id="genre" value="">

	    </div>
	   
	   <input type="submit" data-theme="b" value="Add"/>
	   <a href="#page2" data-role="button">Close</a>
	   </form>
	  </div>
        </div>

        
        
        <!-- =================== ADD SHOUTCAST URL ================ -->
        
        <div data-role="page" id="add_url">
         <div data-role="header">
          <h3>Add shoutcast URL</h3>
         </div>
         <div data-role="content" style="padding: 15px">
          <form action="app.php#page2" data-ajax="false" method="GET">
	   <div data-role="fieldcontain">
	    <label for="sc_url">URL:</label>
	    <input type="text" name="sc_url" id="sc_url" value="">
	    
	    <label for="sc_name">(Optional) Name:</label>
	    <input type="text" name="sc_name" id="sc_name" value="">
	    
	    <label for="sc_genre">(Optional) Genre:</label>
	    <input type="text" name="sc_genre" id="sc_genre" value="">
	   </div>
	   
	   <!--<a href="#page2" data-role="button" data-theme="b">Add</a>-->
	   <input type="submit" data-theme="b" value="Add"/>
	   <a href="#page2" data-role="button">Close</a>
	   </form>
	  </div>
        </div>


        
        <!-- =================== DELETE STATION ================ -->
        
        <div data-role="page" id="really_delete">
         <div data-role="header">
          <h3>Delete station</h3>
         </div>
         <div data-role="content" style="padding: 15px">
           <p>Do you really want to delete this station?</p>
	    <input type="button" onclick="window.location.href = 'app.php?del_item_id=' + document.getElementById('del_item').value + '#page2';" data-theme="b" value="Delete"/>
	    <a href="#page2" data-role="button">Close</a>
	  </div>
        </div>
        
        
        
        <!-- =================== PLAY FOLDER ================ -->
        
        <div data-role="page" id="play_folder">
         <div data-role="header">
          <h3>Play folder</h3>
         </div>
         <div data-role="content" style="padding: 15px">
	    <input type="button" onclick="window.location.href = 'app.php?play_folder=<? echo base64_encode($_GET["root"]); ?>#page1';" data-theme="b" value="Play"/>
	    <a href="#usb" data-role="button">Close</a>
	  </div>
        </div>
        
    </body>
</html>
<?php
include("Mobile_Detect.php");
$detect = new Mobile_Detect();

if($detect->isMobile()) {
 header("Location:app.php");
} else {
 header("Location:pc.html");
}

?> 

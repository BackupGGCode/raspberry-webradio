  var title = document.getElementById("watch-headline-title").innerText;
  var url = document.location.href;
  var req = new XMLHttpRequest();
  req.open("GET", "http://raspberrypi.local/yt.php?play=" + url + "&title=" + title, true);
  req.send(null);
  alert('Sending YouTube video to Raspberry Webradio! This might take a while.'); 

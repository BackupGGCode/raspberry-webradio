/////////////////////////////////////////////////////////////////////
//
// Chrome extension to send YouTube video to Raspberry Webradio
//
// 08.02.2013 - Michael Schwarz
//
/////////////////////////////////////////////////////////////////////

// send video to Raspberry via AJAX
chrome.pageAction.onClicked.addListener(function(tab) {
  var url = tab.url;

  if(url.indexOf("youtube") == -1 || url.indexOf("watch") == -1) {
    alert("Not a YouTube video!"); 
  } else {
    var req = new XMLHttpRequest();
    req.open("GET", "http://raspberrypi.local/yt.php?play=" + url, true);
    req.send(null);
    alert('Sending YouTube video to Raspberry Webradio! This might take a while.');
  }  
});


function checkForValidUrl(tabId, changeInfo, tab) {
  // check if it is a YouTube watch URL
  if (tab.url.indexOf("youtube") > -1 && tab.url.indexOf("watch") > -1) {
    // if so, show action
    chrome.pageAction.show(tabId);
  }
};

// listen for any changes to the URL of any tab.
chrome.tabs.onUpdated.addListener(checkForValidUrl);

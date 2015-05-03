Pebble.addEventListener("showConfiguration",
  function(e) {
   
    //getting platform
    var platform;
    if(Pebble.getActiveWatchInfo) {
      // Available for use!
      platform = Pebble.getActiveWatchInfo().platform;
    } else {
      // Not available, handle gracefully
      platform = 'aplite';
    }
    
    //Load the remote config page
    Pebble.openURL("http://codecorner.galanter.net/pebble/color_shadow/color_shadow_config.html?platform=" + platform);
    
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    
    if (e.response !== '') {
      
      console.log('resonse: ' + decodeURIComponent(e.response));
      
      //Get JSON dictionary
      var settings = JSON.parse(decodeURIComponent(e.response));
      
      //Send to Pebble, persist there
      Pebble.sendAppMessage(
          {
            "KEY_SHADOW_DIRECTION": settings.shadowdirecton,
            "KEY_H1_SHADOW_COLOR": settings.h1shadowcolor,
            "KEY_H2_SHADOW_COLOR": settings.h2shadowcolor,
            "KEY_M1_SHADOW_COLOR": settings.m1shadowcolor,
            "KEY_M2_SHADOW_COLOR": settings.m2shadowcolor,
            "KEY_BG_COLOR": settings.bgcolor,
            "KEY_TIME_COLOR": settings.timecolor
          },
        function(e) {
          console.log("Sending settings data...");
        },
        function(e) {
          console.log("Settings feedback failed!");
        }
      );
    }
  }
);
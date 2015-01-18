Pebble.addEventListener('ready',
  function(e) {
    console.log('JavaScript app ready and running!');
  }
);

Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  Pebble.openURL('http://sno.wball.me/pebble-config.html');
});

Pebble.addEventListener('appmessage', 
	function (e){
		console.log("Received request from Pebble: " + JSON.stringify(e.payload));
		console.log("Sending request to server...");

		
	}
);

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    // Saving OAuth key
    var key = 5;
	var value = 'Some string';

	// Persist write a key with associated value
	localStorage.setItem(key, value);

	// Persist read a key's value. May be null!
	var value = localStorage.getItem(key);


  }
);
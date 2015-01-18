var clientid = '135589284865-leq55m1ga76cjbc1thp4utfgtdfue83i.apps.googleusercontent.com';
var apiKey = 'AIzaSyAdjHPT5Pb7Nu56WJ_nlrMGOAgUAtKjiPM';

var scope = 'https://www.googleapis.com/auth/calendar';

function auth() {
	var config = {
		'client_id': clientid,
		'scope': scope
	};
	gapi.auth.authorize(config, function(token) {
		console.log('login complete');
		console.log(token);
		location.href = 'pebblejs://close#' + encodeURIComponent(JSON.stringify(token));
	})
}

/*window.onload = function() {
	$('#gcal_login').text("Logout from Google Calendar");
}*/
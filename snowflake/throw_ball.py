from apiclient.discovery import build
from oauth2client.client import OAuth2WebServerFlow

import httplib2

def change_event(cal_id: str, month: int, day: int, code: str, action: int, evt_type=None) -> bool:
	# initial 
	# fetch data

	# parse json
	# complete action
	return False
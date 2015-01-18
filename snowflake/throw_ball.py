from apiclient.discovery import build
from oauth2client.client import OAuth2WebServerFlow

import httplib2

def change_event(oauth_key, cal_id, month, day, code, action, evt_type=None):
    """oauth_key: str, cal_id: str, month: int, day: int, code: int, evt_type=None -> bool"""

    # fetch data
    events = service.calendar.events().list(cal_id)
    print events

    # parse json
    # complete action
    return False

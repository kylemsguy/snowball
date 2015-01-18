from apiclient.discovery import build
from oauth2client.client import OAuth2WebServerFlow
from oauth2client import client
import datetime

import httplib2

from snowball.local_config import *

def change_event(access_token, cal_id, month, day, code, action, evt_type=None):
    """oauth_key: str, cal_id: str, month: int, day: int, code: int, evt_type=None -> bool"""
    credentials = client.OAuth2Credentials(access_token, google_client_id, google_client_secret, None, None, "", "snowflake")
    http_auth = credentials.authorize(httplib2.Http())
    service = build(serviceName="calendar", version="v3", http=http_auth)
    # fetch data
    events = service.events().list(calendarId=cal_id, pageToken=None).execute()
    print events
    for event in events['items']:
        print event['summary']


    # parse json
    # complete action
    return False

def quick_add_event(access_token, cal_id, text):
    """oauth_key: str, cal_id: str, month: int, day: int, code: int, evt_type=None -> bool"""
    credentials = client.OAuth2Credentials(access_token, google_client_id, google_client_secret, None, None, "", "snowflake")
    http_auth = credentials.authorize(httplib2.Http())
    service = build(serviceName="calendar", version="v3", http=http_auth)
    # fetch data
    event = service.events().quickAdd(calendarId=cal_id, text=text).execute()
    print event

    return event

def delete_matching_event(access_token, cal_id, course, location, date):
    """oauth_key: str, cal_id: str, month: int, day: int, code: int, evt_type=None -> bool"""
    credentials = client.OAuth2Credentials(access_token, google_client_id, google_client_secret, None, None, "", "snowflake")
    http_auth = credentials.authorize(httplib2.Http())
    service = build(serviceName="calendar", version="v3", http=http_auth)
    # fetch data
    page_token = None
    while True:
        events = service.events().list(calendarId=cal_id, pageToken=page_token,
            timeMin=datetime.datetime(date[2], date[1], date[0]-1).isoformat() + "Z",
            timeMax=datetime.datetime(date[2], date[1], date[0]+1).isoformat()  + "Z").execute()
        for event in events['items']:
            if course.lower() in event["summary"].lower() and location.lower() in event["location"].lower():
                print(event)
                result = service.events().delete(calendarId=cal_id, eventId=event["id"]).execute()
                print(result)
                break
        page_token = events.get('nextPageToken')
        if not page_token:
            break

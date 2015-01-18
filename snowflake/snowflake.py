import re
import time
import datetime
from datetime import date

months = {"Jan":1, "January":1,
             "Feb":2, "February":2,
             "Mar":3, "March":3,
             "Apr":4, "April":4,
             "May":5,
             "Jun":6, "June":6,
             "Jul":7, "July":7,
             "Aug":8, "August":8,
             "Sep":9, "September":9,
             "Oct":10, "October":10,
             "Nov":11, "November":11,
             "Dec":12, "December":12,
             "jan":1, "january":1,
             "feb":2, "february":2,
             "march":3,
             "apr":4, "april":4,
             "jun":6, "june":6,
             "jul":7, "july":7,
             "aug":8, "august":8,
             "sep":9, "september":9,
             "oct":10, "october":10,
             "nov":11, "november":11,
             "dec":12, "december":12,
          }

days = {"This":0, "this":0, "Next":7, "next":7, "Coming":7, "coming":7,
                "Today":0, "today":0,
                "Tomorrow":1, "tomorrow":1,
                "Sunday":0, "sunday":0,
                "Monday":1, "monday":1,
                "Tuesday":2, "tuesday":2,
                "Wednesday":3, "wednesday":3,
                "Thursday":4, "thursday":4,
                "Friday":5, "friday":5,
                "Saturday":6, "saturday":6}

status = {"cancel":0,"cancelled":0, "moved":1, "changed":1, "added":2}

#general_re = 

abs_re = re.compile(r"(Jan|January|Feb|February|Mar|March|Apr|April|May|Jun|June|Jul|July|Aug|August|Sep|September|Oct|October|Nov|November|Dec|December)\.? (\d{0,2})", re.IGNORECASE)
mth_re = re.compile(r"\b(\d?(1st|2nd|3rd|[0456789]th)\b)", re.I)
adj_re = re.compile(r"(today|tomorrow)", re.I)
mod_re = re.compile(r"(this|next|coming)", re.I)
day_re = re.compile(r"(sunday|monday|tuesday|wednesday|thursday|friday|saturday)", re.I)

stat_re = re.compile(r"(cancelled|moved|changed|added)", re.I)

def get_status(text):
    res = stat_re.search(text)
    if not res:
        print("No status found")
    return status[res.group(1)]

# avoid doubly matching similar items inside context
def eat_context(pos):
    return pos + 10

def proc_abs_date(sentdate, abs_date):
    month = months[abs_date.group(1)]
    day = int(abs_date.group(2))
    return (month, day, eat_context(abs_date.end(2)))

def proc_inside_month(sentdate, inside_month):
    return (sentdate.month, int(inside_month.group(1)[:-2]), eat_context(inside_month.end(1)))
def proc_adj_date(sentdate, adjacent_day):
    sentdate += datetime.timedelta(days=days[adjacent_day.group(1)])
    pos = eat_context(adjacent_day.end(1))
    return (sentdate.month, sentdate.day, pos)

matchers = [
    {"exp": abs_re, "proc": proc_abs_date},
    {"exp": mth_re, "proc": proc_inside_month},
    {"exp": adj_re, "proc": proc_adj_date}
    ]

def get_date(sentdate, text, pos=0):
    dates= []
    while pos < len(text):
        date = get_one_date(sentdate, text, pos)
        if date == None:
            break
        pos += date[2]
        dates.append(date)
    return dates

def get_one_date(sentdate, text, pos=0):
    minpos=0xffffff
    minresult=None
    minproc = None
    for matcher in matchers:
        result = matcher["exp"].search(text, pos)
        if result == None:
            continue
        if result.pos < minpos:
            minresult = result
            minproc= matcher["proc"]
    if minresult != None:
        return minproc(sentdate, minresult)
    return None

def get_date2(sentdate, text, pos=0):
    '''greedily match first valid date
    datetime, str -> (month:int, day:int, pos:int)'''
    abs_date = abs_re.search(text, pos)
    if abs_date:
        month = months[abs_date.group(1)]
        day = int(abs_date.group(2))
        return (month, day, eat_context(abs_date.end(2)))
    
    else:
        inside_month = mth_re.search(text, pos)
        if inside_month:
            return (sentdate.month, int(inside_month.group(1)[:-2]), eat_context(inside_month.end(1)))
        
        # relative date mode
        print("Absolute date not found, trying relative")
        adjacent_day = adj_re.search(text, pos)
        # adjacent mode
        if adjacent_day:
            sentdate += datetime.timedelta(days=days[adjacent_day.group(1)])
            pos = eat_context(adjacent_day.end(1))
            return (sentdate.month, sentdate.day, pos)
        # adjacent week
        else:
            adjacent_modifier = mod_re.search(text, pos)
            # update pos if found
            if not adjacent_modifier:
                print("No adjacent modifier")
            else:
                pos = eat_context(adjacent_modifier.end(1))
                
            adjacent_week = day_re.search(text, pos)
            if not adjacent_week:
                # could be next tutorial, raise flag with (0,0)
                print("No specification within week")
                return (0,0, pos)
            else:
                pos = adjacent_week.end(1)
                
            sentdate += datetime.timedelta(days=days[adjacent_modifier.group(1)] + days[adjacent_week.group(1)])
            return (sentdate.month, sentdate.day, pos)




text = """Hi all,
Next week's BME205 tutorial (Jan. 23) is canceled due to the EngSci Education Conference. If you want, you can go to the Tuesday/Thursday tutorials but you need to email the head TA Gabrielle Lam (gabrielle.lam@mail.utoronto.ca )
january 25
You will all be awarded the attendence mark regardless if you choose to attend the Tuesday/Thursday sessions. 
 Richard
"""
text3 = """A quick heads-up that the first Praxis lecture
(tomorrow; Friday at 1300) has been moved to BA1160 (the same room where the Welcome Presentation was held on Wednesday). We're told that this is a "one time change"
and that we should be back in MC102 starting next Monday (the 9th)."""

dates = get_date(datetime.datetime.fromtimestamp(1421514681), text)
print(dates)

dates2 = get_date(datetime.datetime(2013, 9, 5), text3)
print(dates2)


status = get_status(text3)
print(status)
    

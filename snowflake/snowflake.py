import re
import time
import datetime
from datetime import date

month_map = {"Jan":1, "January":1,
             "Feb":2, "Februrary":2,
             "Mar":3, "March":3,
             "Apr":4, "April":4,
             "May":5,
             "Jun":6, "June":6,
             "Jul":7, "July":7,
             "Aug":8, "August":8,
             "Sep":9, "September":9,
             "Oct":10, "October":10,
             "Nov":11, "November":11,
             "Dec":12, "December":12}

relative_map = {"This":0, "this":0, "Next":7, "next":7, "Coming":7, "coming":7,
                "Today":0, "today":0,
                "Tomorrow":1, "tomorrow":1,
                "Sunday":0, "sunday":0,
                "Monday":1, "monday":1,
                "Tuesday":2, "tuesday":2,
                "Wednesday":3, "wednesday":3,
                "Thursday":4, "thursday":4,
                "Friday":5, "friday":5,
                "Saturday":6, "saturday":6}

abs_re = re.compile(r"(Jan|January|Feb|February|Mar|March|Apr|April|May|Jun|June|Jul|July|Aug|August|Sep|September|Oct|October|Nov|November|Dec|December)\.? (\d{0,2})", re.IGNORECASE)

text = """Hi all,
Next week's BME205 tutorial (Jan. 23) is canceled due to the EngSci Education Conference. If you want, you can go to the Tuesday/Thursday tutorials but you need to email the head TA Gabrielle Lam (gabrielle.lam@mail.utoronto.ca )
january 25
You will all be awarded the attendence mark regardless if you choose to attend the Tuesday/Thursday sessions. 
 Richard
"""
text2 = """the tutorial tomorrow will be cancelled"""

text3 = """Next lecture's cancelled"""

def month_day(sentdate, text):
    '''datetime, str -> (int, int)'''
    abs_date = abs_re.findall(text)
    if len(abs_date) > 1:
        print("Multiple dates specified in body, ambiguous")
        print(abs_date)

    if not abs_date:
        print("Switching to relative date")
        adjacent_day = re.search(r"(today|tomorrow)", text, re.IGNORECASE)
        # adjacent mode
        if adjacent_day:
            sentdate += datetime.timedelta(days=relative_map[adjacent_day.group(1)])
        # adjacent week
        else:
            adjacent_modifier = re.search(r"(this|next|coming)", text, re.IGNORECASE)
            adjacent_week = re.search(r"(sunday|monday|tuesday|wednesday|thursday|friday|saturday)", text, re.IGNORECASE)
            
            if not adjacent_modifier:
                print("No adjacent modifier")
            if not adjacent_week:
                # could be next tutorial, raise flag with (0,0)
                print("No specification")
                return (0,0)
            sentdate += datetime.timedelta(days=relative_map[adjacent_modifier].group(1) + relative_map[adjacent_week].group(1))
        return (sentdate.month, sentdate.day)
    else:
        month = month_map[abs_date[0][0]]
        day = int(abs_date[0][1])
        return (month, day)


month, day = month_day(datetime.datetime.fromtimestamp(1421514681), text)
print(month, day)

month2, day2 = month_day(datetime.datetime.fromtimestamp(1421514681), text2)
print(month2,day2)

month3, day3 = month_day(datetime.datetime.fromtimestamp(1421514681), text3)
print(month3, day3)

import os
import sys
import subprocess

executable_path = os.path.join(os.path.dirname(__file__), "../../snowflake/snow")

def run_snowflake(mydate, mytext):
	inputdata = str(mydate.day) + " " + str(mydate.month) + " " + str(mydate.year) + "\n" + mytext
	proc = subprocess.Popen([executable_path], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	stdoutdata, stderrdata = proc.communicate(inputdata)
	parts = stdoutdata.strip().split("\n")
	action = parts[0].split(":", 1)[1].strip()
	course = parts[1].split(":", 1)[1].strip()
	location = parts[2].split(":", 1)[1].strip()
	i = parts[3].strip().split(":")
	date = (int(i[0]), int(i[1]), int(i[2]))
	header = parts[5].strip() if len(parts) >= 6 else ""
	return (action, course, location, date, header)

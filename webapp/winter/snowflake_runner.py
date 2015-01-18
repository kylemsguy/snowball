import os
import sys
import subprocess

executable_path = os.path.join(os.path.dirname(__file__), "../../snowflake/snow")

def run_snowflake(mydate, mytext):
	inputdata = str(mydate.day) + " " + str(mydate.month) + " " + str(mydate.year) + "\n" + mytext
	proc = subprocess.Popen([executable_path], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	stdoutdata, stderrdata = proc.communicate(inputdata)
	parts = stdoutdata.strip().split("\n")
	dates = []
	for ii in parts:
		i = ii.strip().split(":")
		dates.append((int(i[0]), int(i[1]), int(i[2])))
	return dates

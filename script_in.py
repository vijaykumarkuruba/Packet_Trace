import os
import re

fn=raw_input("Enter Dump File name:\n")
if fn:
	fn1=raw_input("Enter operation mode:: (1)p --for packet dumping(2)s --for connection summary (3)t--for RTT  \n")

	matchObj = re.match( r'(.*\-.?)(.*)', fn, re.M|re.I)
	if matchObj:
		a=matchObj.group(1)

	os.system("./packettrace  -"+fn1+"  -r"+ fn +" >"+a+"-"+fn1+".out")

else:
    print "No input given. So taken default file as sample-A.dmp \n" 
    os.system("./packettrace -p -r sample-A.dmp")






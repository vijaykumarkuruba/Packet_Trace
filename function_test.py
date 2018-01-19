import re
import os
f = open("input_function_testing.txt", "r")
os.system('make -f MakeFile inst')
for line in f:
	b = 'dummy'
	a=line
	if (int(line) == 5 or int(line) == 1):
		b=next(f)
        #a=str(a.strip())+"***"+str(b.strip())
        #print a

	os.system('./packet_inst %s %s'%(str(a.strip()),str(b.strip())))
f.close()

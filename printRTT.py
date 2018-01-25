import re
import os
f = open("input_function_testing.txt", "r")
os.system('rm -f *.gc* packet_inst')
os.system('make -f MakeFile inst')
for line in f:
	a=line
	b='dummy'
	try:
		if (int(line) == 4 ):
			os.system('./packet_inst %s %s'%(str(a.strip()),str(b.strip())))
		else:
			pass
	except:
		pass
f.close()

os.system('lcov -c -d . -o printRTT.info')
os.system('genhtml  printRTT.info -o printRTT')

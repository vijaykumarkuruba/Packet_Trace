import re
import os
f = open("input_function_testing.txt", "r")
os.system('rm -f *.gc* packet_inst')
os.system('ls')
os.system('make -f MakeFile inst')
for line in f:
	a=line
	try:
		if (int(line) == 1):
			b=next(f)
			os.system('./packet_inst %s %s'%(str(a.strip()),str(b.strip())))
		else:
			pass
	except:
		pass
f.close()

os.system('lcov -c -d . -o output.info')
os.system('genhtml  output.info -o output')

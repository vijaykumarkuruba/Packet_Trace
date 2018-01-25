import re
import os
f = open("input_function_testing.txt", "r")
os.system('rm -f *.gc* packet_inst ')
os.system('make -f MakeFile inst')
for line in f:
	a=line
	try:
		if (int(line) == 5):
			b=next(f)
			os.system('./packet_inst %s %s'%(str(a.strip()),str(b.strip())))
		else:
			pass
	except:
		pass
f.close()

os.system('lcov -c -d . -o createIP.info')
os.system('genhtml  createIP.info -o createIP')

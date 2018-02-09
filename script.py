

import os
os.system("make clean")
os.system("make")
os.system("./packet")
os.system("lcov -c -d . -o tests.info")
os.system("genhtml tests.info -o TestCases")
#os.system("gcovr -r . --html --html-details -o gcovpacket_test.html")

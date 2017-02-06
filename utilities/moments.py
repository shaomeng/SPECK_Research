#!/opt/apps/gcc-4.8/python-3.5.3/bin/python3

import sys
from   decimal import Decimal
import numpy as np
from   scipy import stats

if len(sys.argv) != 2:
	print ("Please provide a file to process")
	sys.exit(1)

print ("Input file: " + sys.argv[1] )
f = open(sys.argv[1], "r" )
a = np.fromfile( f, dtype=np.float32 )
print ("Length of input: " + str(len(a)) )
mom_1st = stats.moment(a, moment=1, axis=None )
mom_2nd = stats.moment(a, moment=2, axis=None )
mom_3rd = stats.moment(a, moment=3, axis=None )
mom_4th = stats.moment(a, moment=4, axis=None )
print ("Moments of the input (from 1st to 4th):" )
print ("  %.4e" % mom_1st )
print ("  %.4e" % mom_2nd )
print ("  %.4e" % mom_3rd )
print ("  %.4e" % mom_4th )

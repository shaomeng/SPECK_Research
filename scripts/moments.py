#!/glade/u/home/shaomeng/lib_gnu/Python-3.5.3/bin/python3

###  !/opt/apps/gcc-4.8/python-3.5.3/bin/python3

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
mom_5th = stats.moment(a, moment=5, axis=None )
mom_6th = stats.moment(a, moment=6, axis=None )
mom_7th = stats.moment(a, moment=7, axis=None )
mom_8th = stats.moment(a, moment=8, axis=None )
print ("Moments of the input (from 1st to 8th):" )
print ("  %.4e" % mom_1st )
print ("  %.4e" % mom_2nd )
print ("  %.4e" % mom_3rd )
print ("  %.4e" % mom_4th )
print ("  %.4e" % mom_5th )
print ("  %.4e" % mom_6th )
print ("  %.4e" % mom_7th )
print ("  %.4e" % mom_8th )

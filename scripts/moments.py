#!/glade/u/home/shaomeng/lib_gnu/Python-3.5.3/bin/python3

###  !/opt/apps/gcc-4.8/python-3.5.3/bin/python3

import sys
from   decimal import Decimal
import numpy as np
from   scipy import stats
import matplotlib.pyplot as plt

if len(sys.argv) != 5:
	print ("Please provide 4 files to process")
	sys.exit(1)

num_moments = 12

f1 = open(sys.argv[1], "r" )
a1 = np.fromfile( f1, dtype=np.float32 )
b1 = a1.astype(np.float64)
moments1 = [0] * num_moments
print ("Ground truth moments: 1st to 12th")
for i in range(num_moments):
  moments1[i] = stats.moment(b1, moment=i+1, axis=None )
  print ("  %.4e" % moments1[i] )
 
f2 = open(sys.argv[2], "r" )
a2 = np.fromfile( f2, dtype=np.float32 )
b2 = a2.astype(np.float64)
moments2 = [0] * num_moments
print ("16:1 moments: 1st to 12th")
for i in range(num_moments):
  moments2[i] = stats.moment(b2, moment=i+1, axis=None )
  print ("  %.4e" % moments2[i] )
diff2 = [0.0] * num_moments
for i in range(num_moments):
	if moments1[i] != 0.0:
		diff2[i] = (moments2[i]-moments1[i]) / moments1[i]
	else:
		diff2[i] = 0.0
 
f3 = open(sys.argv[3], "r" )
a3 = np.fromfile( f3, dtype=np.float32 )
b3 = a3.astype(np.float64)
moments3 = [0] * num_moments
print ("32:1 moments: 1st to 12th")
for i in range(num_moments):
  moments3[i] = stats.moment(b3, moment=i+1, axis=None )
  print ("  %.4e" % moments3[i] )
diff3 = [0.0] * num_moments
for i in range(num_moments):
	if moments1[i] != 0.0:
		diff3[i] = (moments3[i]-moments1[i]) / moments1[i]
	else:
		diff3[i] = 0.0
 
f4 = open(sys.argv[4], "r" )
a4 = np.fromfile( f4, dtype=np.float32 )
b4 = a4.astype(np.float64)
moments4 = [0] * num_moments
print ("64:1 moments: 1st to 12th")
for i in range(num_moments):
  moments4[i] = stats.moment(b4, moment=i+1, axis=None )
  print ("  %.4e" % moments4[i] )
diff4 = [0.0] * num_moments
for i in range(num_moments):
	if moments1[i] != 0.0:
		diff4[i] = (moments4[i]-moments1[i]) / moments1[i]
	else:
		diff4[i] = 0.0


x = [i+1 for i in range(num_moments) ]

plt.plot( x, diff2, 'r-', label="16:1" )
plt.plot( x, diff3, 'g-', label="32:1" )
plt.plot( x, diff4, 'b-', label="64:1" )

plt.title('Difference in moments of vx ')
plt.xlabel('Order of moments')
plt.ylabel('Difference')
plt.legend()
# plt.show()
plt.savefig('foo.pdf', bbox_inches='tight')



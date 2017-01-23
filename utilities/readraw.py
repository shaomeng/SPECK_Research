#!/usr/local/bin/python3

import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

num_bins = 50

f = open("/Users/shaomeng/Data/TaylorGreen/vx.025.raw.cube0", "r" )
a = np.fromfile( f, dtype=np.float32 )
print ("length of input: " + str(len(a)) )
y, binEdges = np.histogram(a, num_bins, normed=1)
binCenters  = 0.5*(binEdges[1:]+binEdges[:-1])
plt.plot( binCenters, y, 'r--', label="cube1" )

f = open("/Users/shaomeng/Data/TaylorGreen/vx.025.raw.cube10", "r" )
a = np.fromfile( f, dtype=np.float32 )
print ("length of input: " + str(len(a)) )
y, binEdges = np.histogram(a, num_bins, normed=1)
binCenters  = 0.5*(binEdges[1:]+binEdges[:-1])
plt.plot( binCenters, y, 'g-', label="cube2" )

plt.xlabel('vx')
plt.ylabel('Probability')
plt.title('Histogram of vx ground truth')

plt.legend()
plt.show()

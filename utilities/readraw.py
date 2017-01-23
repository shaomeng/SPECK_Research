#!/opt/apps/gcc-4.8/python-3.5.3/bin/python3


####!/usr/local/bin/python3

import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

num_bins = 200

f = open("/flash_buffer/Sam/wx.025.raw", "r" )
a = np.fromfile( f, dtype=np.float32 )
print ("length of input: " + str(len(a)) )
y, binEdges = np.histogram(a, num_bins, normed=1)
binCenters  = 0.5*(binEdges[1:]+binEdges[:-1])
plt.plot( binCenters, y, 'r-', label="ground truth" )

f = open("/flash_buffer/Sam/speck_64to1/wx.025.raw.64to1", "r" )
a = np.fromfile( f, dtype=np.float32 )
print ("length of input: " + str(len(a)) )
y, binEdges = np.histogram(a, num_bins, normed=1)
binCenters  = 0.5*(binEdges[1:]+binEdges[:-1])
plt.plot( binCenters, y, 'g--', label="64:1 SPECK" )

f = open("/flash_buffer/Sam/vdf/wx.025.raw.64to1", "r" )
a = np.fromfile( f, dtype=np.float32 )
print ("length of input: " + str(len(a)) )
y, binEdges = np.histogram(a, num_bins, normed=1)
binCenters  = 0.5*(binEdges[1:]+binEdges[:-1])
plt.plot( binCenters, y, 'b--', label="64:1 VAPOR" )

plt.xlabel('vx')
plt.ylabel('Probability')
plt.title('Histogram of wx ')

plt.legend()
plt.show()

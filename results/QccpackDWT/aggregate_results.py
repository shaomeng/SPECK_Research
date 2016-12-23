import sys

if len(sys.argv) == 1:
  sys.exit("Please pass in at least 1 variable name")
varname = sys.argv[1]
filenames = []
for i in range(0, 6):
  filenames.append( "test.face" + str(i) )

matchstring = varname + ":\n"
rmse = []
maxError = []
maxRelativeError = []
for name in filenames:
  with open( name ) as f:
    lines = f.readlines()
  #next( l for l in lines if l==matchstring )
  idx = lines.index( matchstring )
  for s in lines[idx+2].split():
    try:
      rmse.append( float(s) )
    except ValueError:
      pass
  for s in lines[idx+3].split():
    try:
      maxError.append( float(s) )
    except ValueError:
      pass
  for s in lines[idx+4].split():
    try:
      maxRelativeError.append( float(s) )
    except ValueError:
      pass

print varname + ":"
print "  RMSE             = " + str(max(rmse))
print "  maxError         = " + str(max(maxError))
print "  maxRelativeError = " + str(max(maxRelativeError))

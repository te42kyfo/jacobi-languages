import numpy as np
import matplotlib.pyplot as plt
import time

xsize = 256
ysize = 128
iters = 10


A = np.zeros( (ysize, xsize) )
A[0][:] = 1
A[-1][:] = 1
B = np.copy(A)

t1 = time.clock()
for iter in range(iters):
    for y in range(1,ysize-1):
        for x in range(1,xsize-1):
            A[y,x] = ( B[y+1,x] + 
                       B[y-1,x] + 
                       B[y,x+1] + 
                       B[y,x-1] ) * 0.25
    A,B = B,A

t2 = time.clock()

flops = (xsize-2)*(ysize-2)*iters*4 
time = (t2-t1)

print ( flops )
print ( time )

print ( flops/time * 1.0e-6, )
print ( "Mflop/s" )


plt.imshow(A, interpolation='none')
plt.show()

import numpy as npp
def nonlin (x , deriv=False ):
    if deriv:
        return x*(1-x)
    return 1/(1+npp.exp(-x))
# input dataset 
X = npp.array ([[0,0,1],
              [0,1,1],
              [1,0,1],
              [1,1,1]])
# out put dadaset
y = npp.array ([0,0,1,1]).T
# seed random numbers to make calculation
#  deterministic (just a good practice)
npp. random .seed (1)
# initialize weights randomly with mean 0
syn0 = 2*npp.random.random((3,1)) - 1
for iter in range (10000):
    l0 = X
    l1 = nonlin(npp.dot(l0,syn0))
    l1_error = y - l1
    # multiply how much we missed by the  
    # slope of the sigmoid at the values in l1
    
    
    l1_delta = l1_error * nonlin(l1,deriv=True)
    syn0 = npp.dot(l0.T,l1_delta)
    
print ("Output After Training:" , l1)
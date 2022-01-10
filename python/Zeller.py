#/usr/bin/python
import sys
import math

if len(sys.argv) == 4:
    J = int(sys.argv[1])/100
    K = int(sys.argv[1])%100
    m = int(sys.argv[2])
    q = int(sys.argv[3])
else :
    K = 0
    J = 0
    m = 0
    q = 0
    print "Exit"
    exit()

h = int((q+math.floor((m+1)*26/10)+K+math.floor(K/4)+math.floor(J/4)+5*J)%7)

if h == 1:
    print "Sunday"
elif h == 2:
    print "Monday"
elif h == 3:
    print "Tuesday"
elif h == 4:
    print "Wednesday"
elif h == 5:
    print "Thursday"
elif h == 6:
    print "Friday"
else :
    print "Saturday"

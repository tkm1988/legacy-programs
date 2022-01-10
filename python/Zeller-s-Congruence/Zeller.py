#/usr/bin/python
import sys
import math

def Zeller(Y,m,q):
    return int((q+math.floor((m+1)*26/10)+Y+math.floor(Y/4)+math.floor(Y/100)*6+math.floor(Y/400)) % 7)

def getDayOfTheWeek(h):
    if h == 1:
        return "Sunday"
    elif h == 2:
        return "Monday"
    elif h == 3:
        return "Tuesday"
    elif h == 4:
        return "Wednesday"
    elif h == 5:
        return "Thursday"
    elif h == 6:
        return "Friday"
    else :
        return "Saturday"

if len(sys.argv) == 4:
    Y = int(sys.argv[1])
    m = int(sys.argv[2])
    if m == 1:
        m = 13
    elif m == 2 :
        m =14
    q = int(sys.argv[3])
else :
    Y = 0
    m = 0
    q = 0
    print "Please input \n>python Zeller.py year month day\n"
    exit()

print getDayOfTheWeek(Zeller(Y,m,q))

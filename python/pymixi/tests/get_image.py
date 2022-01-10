#!/usr/bin/env python

import sys, time
import mixi

argv = sys.argv[1:]

if len(argv) < 2:
    sys.exit(1)


uid = argv[0]
passwd = argv[1]
try:
    target = argv[2]
except:
    target = None

try:
    proxy = argv[3]
except:
    proxy = None

print uid
print passwd

m = mixi.MIXI(proxy)
m.login(uid, passwd)

name, image = m.get_image(target)
print name
f = file(name, "wb")
f.write(image)
f.close()


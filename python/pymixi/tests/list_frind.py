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

l = m.list_friend(target)

for i in l:
    print "#"*10
    print "name :" + i["name"].encode("utf-8")
    print "id   :" + i["id"]
    print "link :" + i["link"]
    print "num  :" + str(i["num"])

print "-"*10
print "total:" + str(len(l))


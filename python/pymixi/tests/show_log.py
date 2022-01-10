#!/usr/bin/env python
# -*- coding: euc-jp -*-

import sys, time
import mixi

argv = sys.argv[1:]

if len(argv) < 2:
    sys.exit(1)


uid = argv[0]
passwd = argv[1]
try:
    proxy = argv[2]
except:
    proxy = None

print uid
print passwd

m = mixi.MIXI(proxy)
m.login(uid, passwd)

l = m.show_log()

for i in l:
    print "#"*10
    print "name :" + i["creator"].encode("euc-jp")
    print "id   :" + i["id"]
    print "link :" + i["link"]
    print "date :" + time.strftime("%Y/%m/%d %H:%M", i["date"])

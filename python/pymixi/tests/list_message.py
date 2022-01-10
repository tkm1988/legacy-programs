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

l = m.list_message()

for i in l:
    print "#"*10
    print "from :" + i["creator"].encode("utf-8")
    print "title:" + i["title"].encode("utf-8")
    print "link: " + i["link"]
    print "date :" + i["date"].encode("utf-8")
    print i['content'].encode("utf-8")

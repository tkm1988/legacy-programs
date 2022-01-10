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

maxcount, with_last_comment = 25, True
l = m.new_bbs(maxcount, with_last_comment)

for i in l:
    print "#"*10
    print "title :" + i["title"].encode("utf-8")
    print "link :" + i["link"]
    print "date :" + time.strftime("%Y/%m/%d %H:%M", i["date"])
    print "community :" + i["community"].encode("utf-8", "ignore")
    print "last commentator: " + i["last_comment"]["creator"].encode("utf-8", "ignore")
    print i["last_comment"]["content"].encode("utf-8", "ignore")

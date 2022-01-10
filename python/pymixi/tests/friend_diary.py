#!/usr/bin/env python
# -*- coding: euc-jp -*-

import sys, time
import mixi

argv = sys.argv[1:]

if len(argv) < 2:
    sys.exit(1)

uid = argv[0]
passwd = argv[1]
fid = argv[2]
try:
    proxy = argv[3]
except:
    proxy = None

print uid
print passwd
print fid

m = mixi.MIXI(proxy)
m.login(uid, passwd)

l = m.friend_diary(fid, with_content = True)
print l
for i in l:
    print "#"*10
    print "name :" + i["creator"].encode("utf-8")
    print "id   :" + i["id"]
    print "link :" + i["link"]
    print "title:" + i["title"].encode("utf-8")
    print "date :" + time.strftime("%Y/%m/%d %H:%M", i["date"])
    if i.has_key("content"):
	print "-"*40
	print i['content'].encode("utf-8")

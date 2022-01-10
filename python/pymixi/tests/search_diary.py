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
    word = argv[2]
except:
    word = ""

try:
    proxy = argv[3]
except:
    proxy = None

print uid, passwd, word

m = mixi.MIXI(proxy)
m.login(uid, passwd)

l = m.search_diary(keyword = unicode(word, "euc-jp", "ignore"))
for i in l:
    print "#"*10
    print "name :", i["creator"].encode("utf-8")
    print "id :", i["id"]
    print "link :", i["link"]
    print "title :", i["title"].encode("utf-8")
    #print "date :", time.strftime("%Y/%m/%d %H:%M", i["date"])
    print "-"*40
    print i['summary'].encode("utf-8", "ignore")

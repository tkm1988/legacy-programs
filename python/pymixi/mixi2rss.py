#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, time
import mixi

DATEFMT = '%a, %d %b %Y %H:%M:%S +0900'

XML = '''<?xml version="1.0" encoding="utf-8" ?>
<rss version="2.0" xmlns:dc="http://purl.org/dc/elements/1.1/">
 <channel>
  <title>%s</title>
  <link>%s</link>
  <language>ja</language>
  <docs>http://blogs.law.harvard.edu/tech/rss</docs>
  <lastBuildDate>%s</lastBuildDate>
  <description>%s</description>
__ITEMS__
 </channel>
</rss>
'''

ITEM = '''  <item>
   <title>%s</title>
   <pubDate>%s</pubDate>
   <link>%s</link>
   <guid isPermaLink="true">%s</guid>
   <description>%s</description>
   <dc:creator>%s</dc:creator>
  </item>
'''

FORM = '''<!doctype HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
 <head>
  <title>mixi2rss</title>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
 </head>
 <body>
  <form method="get" action="./mixi2rss">
   <table>
    <tr>
     <td>Mail Address</td>
     <td><input type="text" name="addr" /></td>
    </tr>
    <tr>
     <td>Password</td>
     <td><input type="password" name="passwd" /></td>
    </tr>
    <tr>
     <td>Type</td>
     <td>
      <select name="feedof">
       <option value="new_friend_diary" selected>マイミクシィ最新日記
       <option value="new_bbs">コミュニティ最新書き込み
      </select>
     </td>
    </tr>
    <tr>
     <td>&nbsp;</td>
     <td><input type="submit" value="RSSを表示" /></td>
    </tr>
   </table>
  </form>
 </body>
</html>
'''

def _rm_entref(s):
    for (b, a) in (('&', '&amp;'), ('<', '&lt;'), ('>', '&gt;')):
	s = s.replace(b, a)

    return s

def _new_friend_diary(m):
    title, link = 'マイミクシィ最新日記', 'http://mixi.jp/new_friend_diary.pl'
    pdate = time.strftime(DATEFMT, time.localtime())
    xml = XML % (title, link, pdate, title)

    xmlitems = ''
    _maxitem, _with_content, _with_last_comment = 10, True, True
    items = m.new_friend_diary(_maxitem, _with_content, _with_last_comment)
    for item in items:
	link =  _rm_entref(item['link'].encode('utf-8', 'ignore'))
	if not link.startswith('http://mixi.jp/'):
	    continue
	title = _rm_entref(item['title'].encode('utf-8', 'ignore'))
	pdate = time.strftime(DATEFMT, item['date'])
	creator = _rm_entref(item['creator'].encode('utf-8', 'ignore'))
	desc = _rm_entref(item['content'].encode('utf-8', 'ignore'))

	xmlitems += ITEM % (title, pdate, link, link, desc, creator)

	if item["last_comment"]:
	    title = 'Re: %s（%sの日記）' % (title, creator)
	    comment = item["last_comment"]
	    pdate = time.strftime(DATEFMT, comment['date'])
	    creator = _rm_entref(comment['creator'].encode('utf-8', 'ignore'))
	    id = comment["id"].encode('utf-8', 'ignore')
	    desc = _rm_entref(comment['content'].encode('utf-8', 'ignore'))
	    link = '%s#%s' % (link, id)
	    xmlitems += ITEM % (title, pdate, link, link, desc, creator)

    return xml.replace('__ITEMS__', xmlitems.rstrip())

def _new_bbs(m):
    title, link = 'コミュニティ最新書き込み', 'http://mixi.jp/new_bbs.pl'
    pdate = time.strftime(DATEFMT, time.localtime())
    xml = XML % (title, link, pdate, title)

    xmlitems = ''
    _maxitem, _with_last_comment = 20, True
    items = m.new_bbs(_maxitem, _with_last_comment)
    for item in items:
	link =  _rm_entref(item['link'].encode('utf-8', 'ignore'))
	title = '%s : %s' % (item['title'], item['community'])
	title = _rm_entref(title.encode('utf-8', 'ignore'))
	pdate = time.strftime(DATEFMT, item['date'])
	last_comment = item['last_comment']
	creator = _rm_entref(last_comment['creator'].encode('utf-8', 'ignore'))
	content = last_comment['content'].encode('utf-8', 'ignore')
	desc = _rm_entref(content)
	xmlitems += ITEM % (title, pdate, link, link, desc, creator)

    return xml.replace('__ITEMS__', xmlitems.rstrip())

def mixi2rss(addr = '', passwd = '', feedof = ''):
    if not (addr and passwd and feedof):
	return FORM

    m = mixi.MIXI()
    m.login(addr, passwd)
    if feedof == 'new_friend_diary':
	xml = _new_friend_diary(m)
    elif feedof == 'new_bbs':
	xml = _new_bbs(m)

    return xml

if __name__ == '__main__':
    addr, passwd, feedof = sys.argv[1:4]

    if feedof in ('new_friend_diary', 'new_bbs'):
	print  mixi2rss(addr, passwd, feedof)


# -*- coding: utf-8 -*-

import urllib
from BeautifulSoup import BeautifulSoup
import mixi

ArticleTitle = ''
ArticleBody = ''
AmebaBlogURL = "http://ameblo.jp/are-inachu/"
AmebaBlog = urllib.urlopen(AmebaBlogURL)
sTmp = AmebaBlog.readline()

myMixi = mixi.MIXI(None)

while sTmp:
    if (sTmp.find('<h3 class="title"><!--') != -1):
        sTmp = sTmp.strip(None)
        sTmp = sTmp.strip('<h3 class="title"><!-- google_ad_section_start(name=s2, weight=.1) -->')
        sTmp  = sTmp.strip('<!-- google_ad_section_end(name=s2) --></h3>\n')
        ArticleTitle = sTmp
    elif (sTmp.find('<!-- google_ad_section_start(name=s1, weight=.9) -->') != -1):
        sTmp = AmebaBlog.readline()
        ArticleBody = sTmp.replace('<br />', '\n')

    sTmp = AmebaBlog.readline()
AmebaBlog.close()

myMixi.login("bump_of_beef@hotmail.com", "acidman0")

if ArticleTitle == '':
	ArticleTitle = 'No Title'
if myMixi.post_diary(ArticleTitle, ArticleBody) == False:
	print 'False'

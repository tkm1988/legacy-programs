# -*- coding: utf-8 -*-

import urllib, re, time
from BeautifulSoup import BeautifulSoup
import mixi

ArticleTitle = ''
ArticleBody = ''
AmebaBlogURL = "http://ameblo.jp/are-inachu/"
AmebaBlog = urllib.urlopen(AmebaBlogURL)
html = AmebaBlog.read()

m_usr = "bump_of_beef@hotmail.com"
m_pw = "acidman0"
myMixi = mixi.MIXI(None)
myMixi.login(m_usr, m_pw)

check_time = 0.0

while True:
	if ((time.time() - check_time) >= 1800.0):
		check_time = time.time()
		soup = BeautifulSoup(html)
		cmpl = re.compile('<!--.*?-->')
		tagList = soup.findAll(lambda tag : tag.name == 'h3' and tag.get('class') and 'title' in tag.get('class'))
		ArticleTitle = cmpl.sub('', tagList[0].renderContents())
		
		tagList = soup.findAll(lambda tag : tag.name == 'div' and tag.get('class') and 'subContents' in tag.get('class'))
		ArticleBody = cmpl.sub('', tagList[1].renderContents())
		ArticleBody = ArticleBody[0 : ArticleBody.find('<div')]
		ArticleBody = ArticleBody.replace('<br />', '\n')
		cmpl = re.compile('<img .* />')
		ArticleBody = cmpl.sub('', ArticleBody)
		soup = BeautifulSoup(ArticleBody)
		tagList = soup.findAll('a')
		cmpl = re.compile('<a .*>')
		for i in xrange(len(tagList)):
			ArticleBody = cmpl.sub(tagList[i].get('href').encode('utf-8'), ArticleBody, 1)
		
		sig = u"-----------------------------------------\nド田舎の片隅で独り言を喋る powered by Ameba Blog\n" + AmebaBlogURL
		ArticleBody = ArticleBody + sig.encode('utf-8')# + AmebaBlogURL
		
		if ArticleTitle == '':
			ArticleTitle = 'No Title'
		print myMixi.post_diary(ArticleTitle, ArticleBody)

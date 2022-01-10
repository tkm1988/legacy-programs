# -*- coding: utf-8 -*-

import time
import twitter
import mixi

t_usr = 'tkm1988'
t_pw = 'kt04309'
m_usr = 'bump_of_beef@hotmail.com'
m_pw = 'acidman0'

api = twitter.Api(t_usr, t_pw)
myTimeline = api.GetUserTimeline(t_usr)
VoiceBody = ""
VoiceBodyLog = ""

myMixi = mixi.MIXI(None)
myMixi.login(m_usr, m_pw)

check_time = 0.0

while True:
	if ((time.time() - check_time) >= 25.0):
		i = 0
		myTimeline = api.GetUserTimeline(t_usr)
		check_time = time.time()
		
		while True:
			VoiceBody = myTimeline.pop(i).text.encode('euc-jp')
			
			if (VoiceBody == VoiceBodyLog):
				i = 20
				print 'XV‚³‚ê‚Ä‚¢‚Ü‚¹‚ñD'
				break
				
			if ((VoiceBody.find('@') == -1) or (i > 19)):
				break
			i = i + 1
		
		if (i > 19):
			continue
		
		VoiceBodyLog = VoiceBody
		print myMixi.post_voice(VoiceBody)

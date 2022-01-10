# -*- coding: euc-jp -*-
import unittest
import mixi
import ConfigParser
import time


config = ConfigParser.ConfigParser()
config.read("test.cfg")


def isPtime(ptime):
    if len(ptime) != 9:
        return False
    p = time.mktime(ptime)
    return True

class BaseFunctionTestCase(unittest.TestCase):
    def testYmdhm(self):
        obj = mixi.MIXI()
        self.assertEquals(obj._ymdhm2ptime(u"2007年01月19日 02:34")[:6],
                          (2007, 1, 19, 2, 34, 0))
        self.assertEquals(obj._ymdhm2ptime(u"2007年01月19日&nbsp;02:34")[:6],
                          (2007, 1, 19, 2, 34, 0))
        self.assertRaises(ValueError, obj._ymdhm2ptime, u"2007年01月19日")

    def testMdhm(self):
        pass

    def testUrlToIds(self):
        pass

    def testGetNextLink(self):
        test_html = u"""<html>
<body>
<a href="next">次を表示</a>
<a href="next2">次を表示</a>
</body>
</html>"""
        
    
    def testNodeToText(self):
        test_html = u"""<html>
<body>
<div>
  this is <b>test</b> date.<br/>
  and ...
</div>
</body>
</html>"""

    def testGetAbsUrl(self):
        pass

    def testGetNameNum(self):
        pass

class LoingTestCase(unittest.TestCase):
    def setUp(self):
        self.obj = mixi.MIXI()
    
    def testLogin(self):
        self.obj.login(config.get("global", "email"),
                       config.get("global", "passwd"))

    def testLoginFail(self):
        email = config.get("login-test", "email")
        passwd = config.get("login-test", "passwd")
        self.assertRaises(mixi.LoginFailure,
                          self.obj.login, email, passwd)


class ShowLogTestCase(unittest.TestCase):
    def setUp(self):
        self.obj = mixi.MIXI()
        self.obj.login(config.get("global", "email"),
                       config.get("global", "passwd"))

    def testLog(self):
        logs = self.obj.show_log()
        logCount = int(config.get("show-log-test", "max_log_count"))
        self.assertEquals(len(logs) <= logCount, True)
        for log in logs:
            date = log["date"]
            self.assertEquals(isPtime(date), True)
            cid = int(log["id"])
            self.assertEquals(not log["creator"], False)
            self.assertEquals(log["link"],
                              "http://mixi.jp/show_friend.pl?id=%s" % cid)

class MessageTestCase(unittest.TestCase):
    def setUp(self):
        self.obj = mixi.MIXI()
        self.obj.login(config.get("global", "email"),
                       config.get("global", "passwd"))

    def testListMessage(self):
        messages = self.obj.list_message()
        for msg in messages:
            date = msg["date"]
            # Current version does not support date for message list
            self.assertEquals(isPtime(date), False)
            self.assertEquals(not msg["creator"], False)
            
            
class ImageTestCase(unittest.TestCase):
    def setUp(self):
        self.obj = mixi.MIXI()
        self.obj.login(config.get("global", "email"),
                       config.get("global", "passwd"))

    def testGetImage(self):
        target = config.get("image-test", "target_user", "1")
        name, image = self.obj.get_image(target)
        # TODO

class ProfileTestCase(unittest.TestCase):
    def setUp(self):
        self.obj = mixi.MIXI()
        self.obj.login(config.get("global", "email"),
                       config.get("global", "passwd"))
    
    def testProfile(self):
        profiles = self.obj.get_profile()

class ListFriendTestCase(unittest.TestCase):
    def setUp(self):
        self.obj = mixi.MIXI()
        self.obj.login(config.get("global", "email"),
                       config.get("global", "passwd"))

    def testLisFriend(self):
        friends = self.obj.list_friend()

class NewBBSTestCase(unittest.TestCase):
    def setUp(self):
        self.obj = mixi.MIXI()
        self.obj.login(config.get("global", "email"),
                       config.get("global", "passwd"))
    
    def testNewBbs(self):
        bbs = self.obj.new_bbs(25, True)

class SearchDiaryTestCase(unittest.TestCase):
    def setUp(self):
        self.obj = mixi.MIXI()
        self.obj.login(config.get("global", "email"),
                       config.get("global", "passwd"))

    def testSearch(self):
        result = self.obj.search_diary(keyword="python")

    
        



if __name__ == "__main__":
    unittest.main()


    
    

    

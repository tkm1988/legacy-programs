import urllib
import twitter

class TwitterBot:
    __usr #user name
    __pw #pass-word
    __url #url to get data
    __api
    __status
    __str #twit messeage

    def __init__(self):
        self.usr = ""
        self.pw = ""
        self.url = ""
        self.str = ""

    def __init__(self, usr, pw):
        self.usr = usr
        self.pw = pw
        self.str = ""
        self.api = twitter.Api(usr, pw)

    def __init__(self, usr, pw, url):
        self.usr = usr
        self.pw = pw
        self.url = url
        self.str = ""
        self.api = twitter.Api(usr, pw)

    def __del__(self):
        self.status = api.PostUpdate(u"Good Bye!")

    def setUrl(self, url):
        self.url = url

    def setAccount(self, usr, pw):
        self.usr = usr
        self.pw = pw
        self.api = twitter.Api(usr, pw)
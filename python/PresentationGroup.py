#!/opt/local/bin/python
# -*- coding: utf-8 -*-
import random
import sys
import codecs

def makeGroup(MemberList, GroupNum):
    GroupList = []
    MaxGroupMemberNum = int(len(MemberList) / GroupNum)
    random.seed()
    random.shuffle(MemberList)

    for i in xrange(GroupNum):
        GroupList.append([])

    for member in MemberList:
        AppendFlag = False

        while not(AppendFlag):
            ChosenGroup = random.randint(0, GroupNum)
            AppendFlag = GroupManager(GroupList, ChosenGroup, MaxGroupMemberNum)
        GroupList[ChosenGroup].append(member)

    return GroupList

            
def GroupManager(GroupList, ChosenGroup, MaxGroupMemberNum):
    if len(GroupList[ChosenGroup]) == MaxGroupMemberNum:
        return False
    else :
        return True

if __name__ == '__main__':
    MemberList = [u"小野寺 泰史",
                  u"浅見 友木乃",
                  u"石田 貴靖",
                  u"泉家 怜奈",
                  u"一戸 彩花",
                  u"岩渕 康平",
                  u"薄井 沙織",
                  u"江連 旭",
                  u"岡田 原",
                  u"小野口 歩実",
                  u"尾畑 勝也",
                  u"角 つばさ",
                  u"岸 百合子",
                  u"喜多 敏正",
                  u"木村 大希",
                  u"越石 彩",
                  u"小山 創史",
                  u"今野 由貴",
                  u"笹川 采佳",
                  u"柴田 光雅"]
    GroupNum = 4

    sys.stdout = codecs.getwriter('utf_8')(sys.stdout)
    GroupList =  makeGroup(MemberList, GroupNum)

    for i in xrange(len(GroupList)):
        for j in GroupList[i]:
            print j + ', ',
        print '\n'

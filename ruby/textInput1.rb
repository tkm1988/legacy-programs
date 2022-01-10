#!/usr/bin/ruby

ans = "I have a dream today. I have a dream that one day every valley shall be exalted, every hill and mountain shall be made low, the rough places will be made plain, and the crooked places will be made straight, and the glory of the Lord shall be revealed, and all flesh shall see it together. This is our hope. This is the faith that I go back to the South with. With this faith we will be able to hew out of the mountain of despair a stone of hope. With this faith we will be able to transform the jangling discords of our nation into a beautiful symphony of brotherhood. With this faith we will be able to work together, to pray together, to struggle together, to go to jail together, to stand up for freedom together, knowing that we will be free one day."

f = open("textInput1.txt")

while line = f.gets
  sum = 0.0
  for i in 0..(((line.length < ans.length) ? line.length : ans.length)-1)
    sum = sum + ((ans[i,i+1] == line[i,i+1]) ? 1.0 : 0)
  end
  #p "sum = "+sum.to_s+"   ans.length = "+ans.length.to_s+"   result="+(sum/ans.length*100).to_s
  p ans
  p line
end
f.close

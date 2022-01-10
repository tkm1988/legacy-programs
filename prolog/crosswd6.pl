word(abalone,a,b,a,l,o,n,e).
word(abandon,a,b,a,n,d,o,n).
word(enhance,e,n,h,a,n,c,e).
word(anagram,a,n,a,g,r,a,m).
word(connect,c,o,n,n,e,c,t).
word(elegant,e,l,e,g,a,n,t).

crosswd(H1, H2, H3, V1, V2, V3):-
    word(H1, Wd01, Kwd01, Wd02, Kwd02, Wd03, Kwd03, Wd04),
    word(H2, Wd05, Kwd04, Wd06, Kwd05, Wd07, Kwd06, Wd08),
    word(H3, Wd09, Kwd07, Wd10, Kwd08, Wd11, Kwd09, Wd12),
    word(V1, Wd13, Kwd01, Wd14, Kwd04, Wd15, Kwd07, Wd16),
    word(V2, Wd17, Kwd02, Wd18, Kwd05, Wd19, Kwd08, Wd20),
    word(V3, Wd21, Kwd03, Wd22, Kwd06, Wd23, Kwd09, Wd24).

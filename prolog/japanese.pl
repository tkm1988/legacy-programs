%---------------
% Questuon1 A
%---------------
val(e1, tall, yes).
val(e1, blue_eyes, yes).
val(e1, blond, yes).
val(e1, japanese, yes).
val(e2, tall, yes).
val(e2, blue_eyes, no).
val(e2, blond, yes).
val(e2, japanese, yes).
val(e3, tall, yes).
val(e3, blue_eyes, yes).
val(e3, blond, no).
val(e3, japanese, no).
val(e4, tall, no).
val(e4, blue_eyes, yes).
val(e4, blond, no).
val(e4, japanese, no).
val(e5, tall, no).
val(e5, blue_eyes, no).
val(e5, blond, yes).
val(e5, japanese, yes).
val(e6, tall, no).
val(e6, blue_eyes, no).
val(e6, blond, no).
val(e6, japanese, no).

%---------------
% Question1 B
%---------------
%| ?- dtlearn(japanese, [e1,e2,e3,e4,e5,e6], [blond,blue_eyes,tall], DT).
%	DT = if(blond=yes,yes,no) ? y
%	yes

%---------------
% Question1 C
%---------------
%| ?- dtlearn(blond, [e1,e2,e3,e4,e5,e6], [japanese,blue_eyes,tall], DT).
%	DT = if(japanese=yes,yes,no) ? y
%	yes
%| ?- dtlearn(blue_eyes, [e1,e2,e3,e4,e5,e6], [japanese,blond,tall], DT).
%	no
%| ?- dtlearn(tall, [e1,e2,e3,e4,e5,e6], [japanese,blue_eyes,blond], DT).
%	no
%| ?- dtlearn(tall, [e1,e2,e3,e4,e5,e6], [japanese,blue_eyes,blond,tall], DT).
%	DT = if(tall=yes,yes,no) ? 
%	yes
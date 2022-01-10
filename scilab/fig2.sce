A=[0 0];
B=[1 0];
C=[1 1];

S=[3 0;0 3];

SA=S*A';
SB=S*B';
SC=S*C';

xset('window', 3);
clf();
plot2d([SA(1,1), SB(1,1), SC(1,1), SA(1,1)], [SA(2,1), SB(2,1), SC(2,1), SA(2,1)],rect=[-1,-1,4,4], style=color('red'));
plot2d([A(1,1), B(1,1), C(1,1), A(1,1)], [A(1,2), B(1,2), C(1,2), A(1,2)], rect=[-1,-1,4,4], style=color('blue'));
fig=gca();
p=fig.children.children;
p(1).line_style=1;p(1).thickness=3;
p(2).line_style=1;p(2).thickness=3;

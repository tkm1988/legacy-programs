x = -2:1:5;
y1 = -2*x+2;
y2 = (-(sqrt(3)+0.5)*x+sqrt(5))/(1+sqrt(3)/2);
y3 = (-(sqrt(3)+0.5)*x-sqrt(5))/(1+sqrt(3)/2);

xset('window', 3);
clf();
plot2d(x, y1, style=color('blue'));
plot2d(x, y2, style=color('red'));
plot2d(x, y3, style=color('green'));
//�Q�ƃx�N�g��
m = 10*rand(1,10);
//��̓x�N�g��
x = 1: 1: 10;
//�w�K�W��
x//�w�K��
N = 20000;
//�G���[
E = zeros(N);
//1-size(x)*[0 1]'�܂ł̓�̓x�N�g���ƑΉ�����Q�ƃx�N�g���̒l
W = zeros(size(x)*[0 1]');

for n = 1 : N,
  for i = 1: 10,
    y = zeros(10);
    tmp = 0;
    //���҃x�N�g�������߂�
    for j = 1: 10,
       y(j) = abs(m(j) - x(i));
    end
    index = find(y == min(y));
    
    //�ߖT�m�[�h�����߂�
    lindex = index(1) - 1;
    rindex = index(1) + 1;
    if (lindex(1) == 0) then
      lindex = size(x)*[0 1]';
    end
    if (rindex(1) == size(x)*[0 1]' +1) then
      rindex = 1;
    end
    
    //�w�K
    m(lindex(1)) = m(lindex(1)) +  eta*(x(i) - m(lindex(1)));
    m(index(1)) = m(index(1)) +  eta*(x(i) - m(index(1)));
    m(rindex(1)) = m(rindex(1)) +  eta*(x(i) - m(rindex(1)));
    
    //�G���[�Z�o
    tmp = tmp + abs(m(index) - x(i));
  end
  E(n) = tmp/(size(x)*[0 1]');
end

xset('window', 3)
clf();
plot((1: 1: 10), m, 'k*-');
xtitle('reference vector', 'index', 'amplitude');

xset('window', 4)
clf();
plot((1:1:N), E, 'r-');
xtitle('Error curve line', 'epoch', 'amplitude');

for i = 1: 10,
  y = zeros(10);
  //���҃x�N�g�������߂�
  for j = 1: 10,
     y(j) = abs(m(j) - x(i));
  end
  W(i) = m(find(y == min(y)));
end

xset('window', 5);
clf();
plot((1:1:10), W, 'b*-');
xtitle('not random', 'input vector amplitude', 'output vector amplitude');


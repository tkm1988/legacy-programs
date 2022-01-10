//�Q�ƃx�N�g��
m = 10*rand(1,10);
//��̓x�N�g��
x = 1: 1: 10;
//�w�K�W��
eta = 0.0002;
//�w�K��
N = 20000;
//�G���[
E = zeros(1,N);
//1-size(x)*[0 1]'�܂ł̓�̓x�N�g���ƑΉ�����Q�ƃx�N�g���̒l
W = zeros(1,size(x)*[0 1]');

for n = 1 : N,
  cnt = 1;
  check = [1 2 3 4 5 6 7 8 9 10];
  while(cnt <= 10),
    //y = zeros(10);
    tmp = 0;
    //������p���ē�̓x�N�g���̑I�o����
    i = round(10*rand());
    if (i<1) then,
      i = 1;
      else if(i>10) then,
        i = 10;
      end
    end
    //���ł�1�x�I�΂�Ă���l�Ȃ�΂�蒼��
    if(check(i) == 0) then
      continue;
    end
    check(i) = 0;
    //���҃x�N�g�������߂�
    for j = 1 : 10,
       y(j) = abs(x(i) - m(j));
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
    
    //�G���[�Z�o9
    tmp = tmp + abs(x(i) - m(j));
    
    cnt = cnt + 1;
  end
  E(1,n) = tmp/(size(x)*[0 1]');
  if (n >= N/4 & E(1,n) >= 0 & E(1,n) <= 0.0001) then
    break;
  end
end

xset('window', 0)
clf();
plot((1: 1: 10), m, 'k*-');
xtitle('reference vector', 'index', 'amplitude');

xset('window', 1)
clf();
plot((1:1:N), E(1,:), 'r-');
xtitle('Error curve line', 'epoch', 'amplitude');

for i = 1: 10,
  //���҃x�N�g�������߂�
  for j = 1: 10,
     y(j) = abs(x(i) - m(j));
  end
  W(1,i) = m(find(y == min(y)));
end

xset('window', 2);
clf();
plot((1:1:10), W(1,:), 'b*-');
xtitle('use random', 'input vector amplitude', 'output vector amplitude');


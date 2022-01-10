//################################################
//
// 'SOM' is function mean 'Self-Organization Map'.
//  -Input        : Input Vector
//  -Ref          : Refernce Vector
//  -Dim          : Refernce Vector's Dimention
//  -Error        : Error Each Epoch
//
//################################################

function [Ref, Error, ETA, SIGMA] = som(Input, Dim)
  if(Dim > 2) then,
    error('ERROR! Dim : Invalid Value(Dim = 1 or 2).');
  end
  
  //Initalize
  //Reference Vector
  if(Dim == 1) then,
    Ref = 10 * rand(size(Input, 'r'), size(Input, 'c'));
  else
    //Square Map
    Ref = 10 * rand(floor(size(Input, 'r')/2)^2, size(Input, 'c'));
    xlength = floor(size(Input, 'r')/2);
    ylength = floor(size(Input, 'r')/2);
  end
      
  //Learning Time
  T = 10000;
  
  //Learning-Rate Coefficient
  while(%t),
    A = 10*rand();
    B = 10*rand();
    eta = A/(1 + B);
    if(eta > 0 & eta <= 1) then
      break
    end
  end

  //Neighbor Function
  sigma = size(Input, 'r')/2;
   
  //Error
  Error = zeros(1, T);
  
  //Learning
  for t = 1:T,
    cnt = 1;
    check = 1:1:size(Ref, 'r');
    
    //Choose an index of input-vector isn't chosen until now.  
    while(cnt <= size(Ref, 'r')),
      tmp = 0;
      chose_index = round(10*rand());
      
      if (chose_index < 1) then,
        chose_index = 1;
      else if(chose_index > size(Ref, 'r')) then,
          chose_index = size(Ref, 'r');
        end
      end

      if(check(chose_index) == 0) then,
        continue;
      end
    
      check(chose_index) = 0;
      
      //Choose an index of variable has minimum distance
      for check_index = 1 : size(Ref, 'r'),
         y(check_index) = norm(Input(chose_index,:) - Ref(check_index,:));
      end
  
      min_index = find(y == min(y));
      min_index = min_index(1);
      if(Dim == 2) then,
        C = [fix(min_index / xlength)+1; min_index - fix(min_index / xlength)*ylength];
      end
      
      //近傍関数と学習率係数を用いてニューロンを更新
      if(Dim == 1) then,
        for i = 1 : size(Ref, 'r'),
          Ref(i,:) = Ref(i,:) + eta * exp(-norm(min_index - i)^2/(2*sigma))*(Input(min_index,:) - Ref(i, :));
        end
      else
        for i = 1 : ylength,
          for j = 1 : xlength,
            Ref(i,:) = Ref(i,:) + eta * exp(-norm(C-[j; i])^2/(2*sigma))*(Input(min_index,:) - Ref(i, :));
          end
        end
      end
      
      //Sum (Error_num/epoch)
      tmp = tmp + norm(Input(chose_index) - Ref(check_index));
      
      cnt = cnt + 1;
    end
      
    //Renewal LRC
    eta = A/(t + 1 + B);
    
    //近傍関数の更新
    sigma = sigma - sigma/(t + 1 + B);
    
    //1[epoch]におけるエラー数を計算
    Error(t) = tmp/cnt;
  end
endfunction

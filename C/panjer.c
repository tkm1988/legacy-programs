#include <stdio.h>
#include <math.h>

double g(int i, double h, double lambda){
  return(h*lambda*exp(- lambda * i * h));
}

double panjer(int r, double h, double lambda){
  int i = 0;
  double sum = 0;
  if(r <= 0){
    return exp(- lambda);
  }else{
    for(i = 1; i <= r; ++i){
      sum += ((lambda * (double)i) / (double)r) * g(i, h, lambda) * panjer(r - i, h, lambda);
    }
    return sum;
  }
}

int main(){
  int r = 10;
  double h = 0.5;
  double lambda = 0.5;
  
  printf("Panjer : %lf\n", panjer(r, h, lambda));
  
  return 0;
} 
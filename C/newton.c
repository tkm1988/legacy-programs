#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(int, char**);
double newton(const double*, double*);

int main(int argc, char** argv){
	if(argc < 2){
		printf("arg error : \">./newton [decimal number]\"\n");
		exit(-1);
	}
	
	const double eta = 0.001;
	char *e;
	double x_0 = (double)strtol(argv[1], &e, 10);
	
  printf("result : %f\n", newton(&eta, &x_0));

  return(0);
}

double newton(const double* eta, double* x){
  double x_n = *x - (*x * *x - 2) / (2 * *x);

  if(fabs(x_n - *x) <= *eta){
    return x_n;
  }else{
    return newton(eta, &x_n);
  }
}

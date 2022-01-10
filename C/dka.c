#include<stdio.h>
#include<math.h>
#include<complex>
using namespace std;
typedef complex<double> Complex;
#define pai 3.1415926535
int dka(complex<double>[],complex<double>[],int);
void print(Complex);

int main(){
	complex<double> z[4];
	complex<double> a[4];
	int n = 0;
	double def = 0;
	int i = 0;
	int x = 0;

//ŒW”‚Ì“ü—Í
	scanf("%lf",&a[0]);
	scanf("%lf",&a[1]);
	scanf("%lf",&a[2]);
	scanf("%lf",&a[3]);

//ŒW”‚Ìâ‘Î’l‚ğæ‚Á‚½‚à‚Ì‚Ì’†‚Å‚ÌÅ‘å’l{‚P‚Ææ”‚ÌŒŸõ
	for(i = 0;i < 4; i++){
			if(abs(a[i]) > abs(a[i-1])){
				def = abs(a[i]);
			}
		n++;
	}
	
	n = n  - 1;
	def = def + (double)1;
//‰Šú’l‚Ìİ’è
	z[0] =Complex((double)(def*cos(8)),(double)(def*sin(8)));
	z[1] =Complex((double)(def*cos(8+((2*pai)/3))),(double)(def*sin(8+((2*pai)/3))));
	z[2] =Complex((double)(def*cos(8+((4*pai)/3))),(double)(def*sin(8+((4*pai)/3))));
	printf("Œ‹‰Ê‚Ì•\¦\n");

	x = dka(z,a,n);
//‰ğ‚ÌŒ‹‰Ê
	if(x == !0){
		printf("‰ğ‚ª‹‚ß‚ç‚ê‚È‚©‚Á‚½\n");
	}else{
		printf("‰ğ‚ª‹‚ß‚ç‚ê‚½\n");
	}
}

int dka(complex<double>z[],complex<double>a[],int n){
	int k = 0;
	int j = 0;
	complex<double> sum_res[4];
	complex<double> mul_res[4];
	complex<double> z2[4];
	complex<double> dif[4];
	complex<double> ans[4];

	int i =  0;

	for(k = 0;k < 101;k++){
		mul_res[0] = Complex(1,0);
		mul_res[1] = Complex(1,0);
		mul_res[2] = Complex(1,0);
		mul_res[3] = Complex(1,0);

//•ªq‚ÌŒvZ
		for(i = 1;i <= n;i++){
			sum_res[i] = a[0] * pow(z[i],(double)n) + a[1] * pow(z[i],(double)(n-1)) + a[2] * pow(z[i],(double)(n-2)) + a[3];
		}
//•ª•ê‚ÌŒvZ
		for(i = 1;i <= n;i++){
			for(j = 1;j <= n;j++){
				if(i != j)mul_res[j] *= (z[j] - z[i]);
			}
		}
//DKA‚ÌŒvZ
		for(j = 1;j <= n;j++){
			z2[j] = z[j] - (sum_res[j]/mul_res[j]);
		}
//·‚ÌŒvZ
		for(j = 1;j <= n;j++){
			dif[j] = z2[j] - z[j];
			print(z2[k]);
			if(abs(dif[j]) <  pow((double)10,(double)-10)){
				ans[j] = z2[j];
			}
		}
		printf("\n");

//Še‰ğ‚ª‚P‚OO|‚P‚OˆÈ‰º‚É‚È‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
		if(abs(dif[1]) < pow((double)10,(double)-10) && abs(dif[2]) < pow((double)10,(double)-10) && abs(dif[3]) < pow((double)10,(double)-10)){

			printf("‰ğ1\n");
				print(ans[1]);
			printf("‰ğ2\n");
				print(ans[2]);
			printf("‰ğ3\n");
				print(ans[3]);
			printf("\n");
				return 0;
		}
		
		for(j = 0;j <= 3;j ++){
			z[j] = z2[j];
		}
	}
//ƒGƒ‰[ˆ—
return !0;
}

//•\¦ŠÖ”
void print(Complex ans){
	if (ans.imag() >= 0.0){
		printf("%lf,%lf,",ans.real(),ans.imag());
	}else{
		printf("%lf,-%lf,",ans.real(),-ans.imag());
	}
}


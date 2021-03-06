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

//係数の入力
	scanf("%lf",&a[0]);
	scanf("%lf",&a[1]);
	scanf("%lf",&a[2]);
	scanf("%lf",&a[3]);

//係数の絶対値を取ったものの中での最大値＋１と乗数の検索
	for(i = 0;i < 4; i++){
			if(abs(a[i]) > abs(a[i-1])){
				def = abs(a[i]);
			}
		n++;
	}
	
	n = n  - 1;
	def = def + (double)1;
//初期値の設定
	z[0] =Complex((double)(def*cos(8)),(double)(def*sin(8)));
	z[1] =Complex((double)(def*cos(8+((2*pai)/3))),(double)(def*sin(8+((2*pai)/3))));
	z[2] =Complex((double)(def*cos(8+((4*pai)/3))),(double)(def*sin(8+((4*pai)/3))));
	printf("結果の表示\n");

	x = dka(z,a,n);
//解の結果
	if(x == !0){
		printf("解が求められなかった\n");
	}else{
		printf("解が求められた\n");
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

//分子の計算
		for(i = 1;i <= n;i++){
			sum_res[i] = a[0] * pow(z[i],(double)n) + a[1] * pow(z[i],(double)(n-1)) + a[2] * pow(z[i],(double)(n-2)) + a[3];
		}
//分母の計算
		for(i = 1;i <= n;i++){
			for(j = 1;j <= n;j++){
				if(i != j)mul_res[j] *= (z[j] - z[i]);
			}
		}
//DKAの計算
		for(j = 1;j <= n;j++){
			z2[j] = z[j] - (sum_res[j]/mul_res[j]);
		}
//差の計算
		for(j = 1;j <= n;j++){
			dif[j] = z2[j] - z[j];
			print(z2[k]);
			if(abs(dif[j]) <  pow((double)10,(double)-10)){
				ans[j] = z2[j];
			}
		}
		printf("\n");

//各解が１０＾−１０以下になっているかどうか
		if(abs(dif[1]) < pow((double)10,(double)-10) && abs(dif[2]) < pow((double)10,(double)-10) && abs(dif[3]) < pow((double)10,(double)-10)){

			printf("解1\n");
				print(ans[1]);
			printf("解2\n");
				print(ans[2]);
			printf("解3\n");
				print(ans[3]);
			printf("\n");
				return 0;
		}
		
		for(j = 0;j <= 3;j ++){
			z[j] = z2[j];
		}
	}
//エラー処理
return !0;
}

//表示関数
void print(Complex ans){
	if (ans.imag() >= 0.0){
		printf("%lf,%lf,",ans.real(),ans.imag());
	}else{
		printf("%lf,-%lf,",ans.real(),-ans.imag());
	}
}


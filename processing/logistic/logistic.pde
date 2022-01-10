void setup(){
  size(400, 350);
  background(0);
}

int n = 25;  //世代数
float diameter = 2.0; //円の直径
float x = 0.1;  //個体数の初期値
float a = 2.5;  //繁殖率

void draw(){
 smooth();
 noStroke();
 
 if(x > 1.0){
   background(0);
   x = 0.4;
 }
 if(a <= 4.0){
   for(int i = 1; i <= n; i++){
     x = a * x * (1 - x);
   }
   ellipse(260.0*(a - 2.5), height - x * 200.0, diameter, diameter);
   a += 0.001; 
 }else{
   fill(random(255), random(255), random(255));
   x += 0.1;
   a = 2.5;
 }
}

#include <iostream>
#include <fstream>

using namespace std;

int main(void);

int main()
{
  int const sSleeve = 0;
  int const lSleeve = 1;
  ofstream ofs("simudata.txt");

  for(int hue = 0; hue <= 360; ++hue){
    cout << hue << endl;
    for(int sat = 0; sat <= 100; ++sat){
        for(int val = 0; val <= 100; ++val){
            for(int neck = 0; neck <= 15 ; ++neck){
              for(int point = 0; point < 8; ++point){
                for(int pColor = 0; pColor < 9; ++pColor){
                  ofs << hue << "\t" << sat << "\t" << val << "\t" << neck << "\t" << point << "\t" << pColor << "\t" << sSleeve << endl;
                  ofs << hue << "\t" << sat << "\t" << val << "\t" << neck << "\t" << point << "\t" << pColor << "\t" << lSleeve << endl;
                }
              }
            }
        }
    }
  }

  ofs.close();
  return 0;
}

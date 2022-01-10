#include <cstring>
#include <iostream>
#include <vector>
#include <boost/random.hpp>
#include "rnd_shuffle.h"
#include "hopfield.hpp"

using namespace std;
using namespace boost;

int main(int, char **);
int main(int argc, char **argv){
#include <numbers>
  int const neuron_num = 63;
  int lt = 0;
  int num = 0;
  int count = 1;
  int *init_state = NULL;
  list<double> energy;
  std::vector<int * > input;
  std::vector<int> state;
  hopfield hf(neuron_num);
  rnd_shuffle rs(9);
  std::vector<int> rsv = rs.getshuffled();
  std::vector<int>::iterator it = rsv.begin();
  mt19937 gen((unsigned long)time(0));
  uniform_smallint<> dst(0, 100);
  variate_generator<mt19937 &, uniform_smallint<> >rand(gen, dst);

  //Process command line arguments that were input by users.
  if(argc != 3){
    cout << ">./hopfield1 [learning time] [Number of images which you let this program memorize]" << endl;
    exit(-1);
  }else{
    lt = (int)strtol(argv[1], NULL, 10);
    num =(int)strtol(argv[2], NULL, 10);
    
    if(lt < 0){
      cout << "Error : argv[1] : Out of Range[1, 2, 3, ...]" << endl;
      exit(-1);
    }
    if(num <= 0 || num > 9){
      cout << "Error : argv[2] : Out of Range[1 - 9]" << endl;
      exit(-1);
    }

    for(int i = 0; i < num; ++i, ++it){
      cout << "choose image is " << *it << endl;
      switch(*it){
      case 0:input.push_back(zero);break;
      case 1:input.push_back(one);break;
      case 2:input.push_back(two);break;
      case 3:input.push_back(three);break;
      case 4:input.push_back(four);break;
      case 5:input.push_back(five);break;
      case 6:input.push_back(six);break;
      case 7:input.push_back(seven);break;
      case 8:input.push_back(eight);break;
      case 9:input.push_back(nine);break;
      default:input.push_back(zero);break;
      }
    }
  }

  //Choose (initial state)
  switch(*it){
  case 0:init_state = zero;break;
  case 1:init_state = one;break;
  case 2:init_state = two;break;
  case 3:init_state = three;break;
  case 4:init_state = four;break;
  case 5:init_state = five;break;
  case 6:init_state = six;break;
  case 7:init_state = seven;break;
  case 8:init_state = eight;break;
  case 9:init_state = nine;break;
  default:init_state = zero;break;
  }

  //Make an image that contains noises.
  for(int i = 0; i < neuron_num; ++i){
    if(rand() < 10){
      switch(init_state[i]){
      case 0:init_state[i]=1;break;
      case 1:init_state[i]=0;break;
      default:init_state[i] = state[i];
      }
    }
  }
  
  cout << "Initial State:" << endl;
  for(int i = 0; i < neuron_num; ++i){
    cout << " " << init_state[i];
    if(((i+1)%7) == 0){
      cout << endl;
    }
  }

  hf.setWeight(&input);
  hf.setState(init_state);
  energy = hf.recognize(lt);
  
  state = hf.getState();

  //Output (result)
  cout << "Energy : " << endl;
  count = 0;
  for(list<double>::iterator lit = energy.begin(); lit != energy.end(); ++lit, ++count){
    if(count < 3 || count > (lt - 3)){
      cout << "\t" << *lit << endl;
    }
    if(count == 3 && lt > 4){
      cout << "\t..." << endl;
    }
  }

  cout << "State : " << endl;
  count = 1;
  for(std::vector<int>::iterator vit = state.begin(); vit != state.end(); ++vit, ++count){
    cout << " " << *vit;
    if(count % 7 == 0){
      cout << endl;
    }
  }

  return 0;
}

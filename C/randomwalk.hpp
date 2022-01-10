#include <vector>

using namespace std;

template <class T>
class randomwalk{
public:
  randomwalk(int, int);
  ~randomwalk();
  T getMean();
  vector<T>* getOrbit();
private:
  vector<T> x;
};

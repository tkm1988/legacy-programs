#include <list>
#include <boost/random.hpp>
#include "simplegene.hpp"

class iGA
{
public:
  iGA(std::list<simplegene> *);
  ~iGA();
  void			crossover(std::list<int> *);
  void			mutation(void);
  void			mutation(int);
  std::list<simplegene> getData(void);
  void                  swap(std::list<simplegene> *);
private:
  unsigned int		terminate_point;
  std::list<simplegene> GeneList;
  std::list<simplegene> Parents;
  std::list<simplegene> Offsprings;
};

iGA::iGA(std::list<simplegene> *GeneList)
{
  this.GeneList.swap(GeneList);
  Persons.clear();
  Offsprings.clear();
}

iGA::~iGA()
{
}

void iGA::crossover(std::list<int> *TargetIndex)
{
}

void iGA::mutation()
{
  mt19937 gen(static_cast<unsigned int>(std::time(0)));
  uniform_smallint<> dist(0, static_cast<int>(Offsprings.size()));
  varigate_generator<mt19937&, uniform_smallint<> > random(gen, dist);

  Offsprings.at(random()).mutation();

  GeneList = Parents.splice(Parents.end(), Offsprings);
}

void iGA::mutation(int select)
{
  if(select >= 0)
  {
    Offsprings.at(select).mutation();
    GeneList = Parents.splice(Parents.end(), Offsprings);
  } 
}

std::list<simplegene> iGA::getData()
{
  return GeneList;
}

void iGA::swap(std::list<simplegene> *Target)
{
  GeneList.swap(Target);
}

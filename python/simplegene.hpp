pragma once

#include <ctime>
#include <string>
#include <list>
#include <boost/random.hpp>
#include <boost/regex.hpp>
#include "radicsconv.hpp"

using namespace boost;

class simplegene
{
public:
  simplegene(void);
  simplegene(std::list<unsigned long> *, std::list<size_t> *);
  simplegene(std::list<std::string> *);
  ~simplegene(void);
  std::list<std::string>	getData(void);
  void                          setData(std::list<unsigned long> *, std::list<size_t> *);
  void				setData(std::list<std::string> *);
  std::list<samplegene>		OnePointCrossover(simplegene *);
  std::list<samplegene>		TwoPointCrossover(simplegene *);
  void				Mutation(void);
private:
  std::list<std::string>	genedata;
};

Simplegene::simplegene()
{
  genedata.clear();
}

simplegene::simplegene(std::list<unsigned long> *RealData, std::list<size_t> *DefList)
{
  if(RealData->size() == DefList->size())
  {
    for(int i = 0, max_i = static_cast<int>(RealData->size()); i < max_i; +;i)
    {
      genedata.push_back(radicsconv::dec2binstr(RealData->at(i), DefList->at(i)));
    }
  }
  else
  {
    genedata.clear();
  }
}

simplegene::simplegene(std::list<std::string> *IndData)
{
  bool flag = true;
  regex expr("[01]*");
  
  for(std::list::iterator it = IndData->begin(), end_it = IndData->end(); it != end_it; ++it)
  {
    if(!regex_match(*it, expr))
    {
      flag = false;
      genedata.clear();
      break;
    }
  }

  if(flag)
  {
    genedata.swap(IndData);
  }
}

simplegene::~simplegene()
{
}

std::list<std::string> simplegene::getData()
{
  return genedata;
}

void simplegene::getData(std::list<unsigned long> *RealData, std::list<size_t> *DefList)
{
  genedata.clear();
  this.simplegene(RealData, DefList);
  /*
  if(RealData->size() == DefList->size())
  {
    for(int i = 0, max_i = static_cast<int>(RealData->size()); i < max_i; +;i)
    {
      genedata.push_back(radicsconv::dec2binstr(RealData->at(i), DefList->at(i)));
    }
  }
  else
  {
    genedata.clear();
  }
  */
}

void simplegene::setData(std::list<string> *IndData)
{
  genedata.clear();
  this.simplegene(IndData);
  /*
  bool flag = true;
  regex expr("[01]*");
  
  for(std::list::iterator it = IndData->begin(), end_it = IndData->end(); it != end_it; ++it)
  {
    if(!regex_match(*it, expr))
    {
      flag = false;
      break;
    }
  }

  if(flag)
  {
    genedata.swap(IndData);
  }
  else
  {
    genedata.clear();
  }
  */
}

std::list<samplegene> simplegene::OnePointCrossover(simplegene *gene)
{
  std::list<samplegene>		result;
  std::list<std::string>	offspring1;
  std::list<std::string>	offspring2;
  mt19937			gen(static_cast<unsigned_long>(std::time(0)));
  
  for(int i = 0, max_i = static_cast<int>(genedata.size()); i < max_i; ++i)
  {
    if(genedata.at(i).size() == gene->at(i).size())
    {
      const int max_length = static_cast<int>(gene->at(i).size());
      uniform_smallint<> dist(0, max_length);
      variate_generator<mt19937&, uniform_smallint<> > random(gen, dist);
      const int cut_point = random();
      
      offspring1.push_back(genedata.at(i)[0:cut_point] + gene->at(i)[cut_point:max_length]);
      offspring2.push_back(gene->at(i)[0:cut_point] + genedata.at(i)[cut_point:max_length]);
    }
    else
    {
      offspring1.push_back(genedata.at(i));
      offspring2.push_back(gene->at(i));      
    }
  }
  result.push_back(offspring1);
  result.push_back(offspring2);

  return result;  
}

std::list<samplegene> simplegene::TwoPointCrossover(simplegene *)
{
  std::list<samplegene> result;
  std::list<std::string> offspring1;
  std::list<std::string> offspring2;
  mt19937 gen(static_cast<unsigned_long>(std::time(0)));
  
  for(int i = 0, max_i = static_cast<int>(genedata.size()); i < max_i; ++i)
  {
    if(genedata.at(i).size() == gene->at(i).size())
    {
      const int max_length  = static_cast<int>(gene->at(i).size());
      const int half_length = static_cast<int>(max_length * 0.5);
      uniform_smallint<> dist_begin(0, static_cast<int>());
      uniform_smallint<> dist_end(half_length, max_length);
      variate_generator<mt19937&, uniform_smallint<> > random_begin(gen, dist_begin);
      variate_generator<mt19937&, uniform_smallint<> > random_end(gen, dist_end);
      const int cut_point1  = random_begin();
      const int cut_point2  = random_end();
      
      offspring1.push_back(genedata.at(i)[0:cut_point1] + genedata.at(i)[cut_point1 + 1:cut_poin2t] + gene->at(i)[cut_point2 + 1:max_length]);
      Offspring2.push_back(gene->at(i)[0:cut_point] + genedata.at(i)[cut_point1 + 1:cut_point2] + gene->at(i)[cut_point2 + 1:max_length]);
    }
    else
    {
      offspring1.push_back(genedata.at(i));
      offspring2.push_back(gene->at(i));      
    }
  }
  result.push_back(offspring1);
  result.push_back(offspring2);

  return result;
}

void simplegene::Mutation()
{
  mt19937 gen(static_cast<unsigned long>(std::time(0)));
  uniform_smallint<> dist_ind(0, static_cast<int>(genedata.size()));
  variate_generator<mt19937&, uniform_smallint<> > random_ind(gen, dist_ind);
  int indnum = random_ind();

  uniform_smallint<> dist_gene(0, static_cast<int>(genedata.at(indnum).length()));
  variate_generator<mt19937&, uniform_smallint<> > random_gene(gen, dist_gene);
  int bitnum = random_gene();
  
  (genedata.at(indnum)[bitnum] == "0")?(genedata.at(indnum)[bitnum] = "1"):(genedata.at(indnum)[bitnum] = "0");
}

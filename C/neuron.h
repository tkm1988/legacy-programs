#pragma once

#include <ctime>
#include <boost/random.hpp>
#include <boost/numeric/ublas/vector.hpp>

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef boost::numeric::ublas::vector<double> dvect;

class neuron{
private:
	int coord;
	dvect weight;
	double net;
	double sigma;
public:
	neuron(int, int);
	void setWeight(dvect);
	void setWeight(double, int);
	void setNet(double);	
	void setSigma(double);
	int getCoord();
	double getNet();
	double getSigma();
	double getWeight(int);
	dvect getWeight();
};

neuron::neuron(int order, int coord){
	mt19937 gen(static_cast<unsigned long>(time(0)));
	uniform_real<> dst(0, 1);
	variate_generator<mt19937, uniform_real<> > rand(gen, dst);

	this->coord = coord;
	this->net = 0.0;
	weight.resize(order);
	for(int i = 0; i < order; ++i){
		weight(i) = rand();
	}
}
void neuron::setWeight(dvect new_weight){
	weight = new_weight;
}
void neuron::setWeight(double new_weight, int index){
	weight(index) = new_weight;
}
void neuron::setNet(double new_net){
	net = new_net;
}
void neuron::setSigma(double new_sigma){
	sigma = new_sigma;
}
int neuron::getCoord(){
	return coord;
}
double neuron::getNet(){
	return net;
}
double neuron::getSigma(){
	return sigma;
}
dvect neuron::getWeight(){
	return weight;
}
double neuron::getWeight(int num){
	return weight(num);
}

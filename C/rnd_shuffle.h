#pragma once

#include <iostream>
#include <ctime>
#include <boost/random.hpp>
template < typename T >

class Increment{
    T val_;
public:
    Increment( const T& init ) : val_( init ){}
    T operator()(){ return val_++; }
};

class Random{
public:
	boost::mt19937 gen;
	boost::uniform_int<int> dst;
	boost::variate_generator< boost::mt19937, boost::uniform_int<int> > rand;
	
	Random( int num ):// call instance:
	gen( static_cast<unsigned long>(std::time(0)) ), dst( 0, num ), rand( gen, dst ) {}
	
	std::ptrdiff_t operator()( std::ptrdiff_t arg ) {
		return static_cast< std::ptrdiff_t >( rand() );
	}
};

class rnd_shuffle{
private:
	int num;
	std::vector<int> vctr;
public:
	rnd_shuffle(int);
	std::vector<int> getshuffled();
};

rnd_shuffle::rnd_shuffle(int num){
	this->num = num;
	Random rnd(num - 1);

	std::generate_n(std::back_inserter(vctr), num, Increment<int>(0));
	std::random_shuffle(vctr.begin(), vctr.end(), rnd);
}

std::vector<int> rnd_shuffle::getshuffled(){
	return vctr;
}

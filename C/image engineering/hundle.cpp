#include<iostream>#include"handlepnm.hpp"#include"perceptron.hpp"int main(){	HandlePnm p("helvetica.pgm", 28);	Perceptron per(64, 1, 64, 2);	std::cout << "Header : " << p.getType() << std::endl;	std::cout << "Width : " << p.getWidth() << std::endl;	std::cout << "Height : " << p.getHeight() << std::endl;	std::cout << "Input Neuron Number : " << per.getInNeuronNum() << std::endl;	std::cout << "Hidden Neuron Number : " << per.getHidNeuronNum() << std::endl;	std::cout << "Hidden Layer Number : " << per.getHidLayerNum() << std::endl;	std::cout << "Output Neuron Number : " << per.getOutNeuronNum() << std::endl;	std::cout << "End" << std::endl;	return 0;}
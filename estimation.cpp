#include "estimation.h"
#include<iostream>

//оценка и корреляция
double getDistribution(std::vector<uint8_t>& data){
	int N = 8*data.size();
	int count = 0;
	for(int i = 0; i < data.size(); i++){
		uint8_t buffer = data[i];
		for(int j = 0; j < 8; j++){
			uint8_t b = (buffer >> j) &1;
			count += b;
		}
	}
	return (double)count/N;
}

double countCorell(std::vector<uint8_t>& plain, std::vector<uint8_t>& chifer){
	int N = 8*chifer.size();
	int corell = 0;
	for(int i = 0; i < chifer.size() - plain.size(); i++){
		plain.push_back(' ');
	}
	for(int i = 0; i < chifer.size(); i++){
		uint8_t p = plain[i];
		uint8_t c = chifer[i];
		for(int j = 0; j < 8; j++){
			uint8_t tmpP = (p >> j) & 1;
            uint8_t tmpC = (c >> j) & 1;
            corell += (2 * tmpP - 1) * (2 * tmpC - 1);
		}
	}
	std::cout << corell << std::endl;
	std::cout << N << std::endl;
	return (double)corell/N;
}
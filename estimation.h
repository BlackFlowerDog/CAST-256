#pragma once
#include<vector>
#include<cstdint>

double getDistribution(std::vector<uint8_t>& data);
double countCorell(std::vector<uint8_t>& plain, std::vector<uint8_t>& cipher);

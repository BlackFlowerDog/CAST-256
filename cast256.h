#pragma once
#include<vector>
#include<cstdio>
#include "blocks.h"

void getBlock(uint8_t* buffer128, std::vector<uint8_t>& data, int index);
void writeBlock(std::vector<uint8_t>& data, uint8_t* buffer128);
std::vector<uint8_t> cast256(std::vector<uint8_t>& data, char* action, char* CFB);
uint32_t* initKey(uint32_t*** subkeysKrKm);
uint32_t*** create3Xarray (int m, int n, int l);
void free(uint32_t*** arr, int m, int n);
uint32_t*** initTrTm();
void w(int round, uint32_t* key, uint32_t*** TrTm);
void KrKm(int round, uint32_t* key, uint32_t*** KrKm);
uint32_t f1(uint32_t key, uint32_t Tr, uint32_t Tm);
uint32_t f2(uint32_t key, uint32_t Tr, uint32_t Tm);
uint32_t f3(uint32_t key, uint32_t Tr, uint32_t Tm);
void Q(uint32_t* data, int round, uint32_t*** KrKm);
void nQ(uint32_t* data, int round, uint32_t*** KrKm);
void applyKey(uint8_t* data, uint32_t*** KrKm, uint8_t* res);
void xOr(uint8_t* a, uint8_t* b, uint8_t* res);
uint32_t* keyGenerate(int size);
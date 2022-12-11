#pragma once
#include<iostream>
#include<fstream>
#include<vector>

#define BMP 19778
#pragma pack(push, 1)
typedef struct{
	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReversed1;
	unsigned short bfReversed2;
	unsigned int bfOffBits;
}BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct S_BMP{
	unsigned int bi_Size;
	unsigned int bi_Width;
	unsigned int bi_Height;
	unsigned short bi_Planes;
	unsigned short bi_BitCount;
	unsigned int bi_Compression;
	unsigned int bi_SizeImage;
	int bi_X;
	int bi_Y;
	unsigned int bi_ClrUsed;
	unsigned int bi_ClrImportant;
}BITMAPINFO;
#pragma pack(pop)

BITMAPFILEHEADER readFileHeader(FILE* file);
BITMAPINFO readFileInfo(FILE* file);
std::vector<uint8_t> getData(FILE* file);
void writeData(FILE* file, std::vector<uint8_t>& data);
void writeData(FILE* file, std::vector<uint8_t>& data, BITMAPINFO fi, BITMAPFILEHEADER fh);
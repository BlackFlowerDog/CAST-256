#include"cast256.h"
#include "estimation.h"
#include "rwfile.h"

#include<iostream>
#include<cstring>
#include<exception>

int main(int argc, char** argv){
	try {
		if(argc != 6){
			throw "not enough arguments";
		}
		char* origfilename = argv[1];//имя шифруемого файла
		char* resultfilename = argv[2];//имя файла, в который запишется результат шифрования
		char* filetype = argv[3];//текстовый или картинка: -t или -i
		char* action = argv[4];//шифрование или дешифрование: -e или -d
		char* key = argv[5];//используется ли режим шифрования CFB: -y (yes) или -n (n)

		FILE* ofile;
		FILE* rfile;
		ofile = fopen(origfilename, "rb");
		rfile = fopen(resultfilename, "wb");

		if(ofile == NULL || rfile == NULL) {
			throw "FILE reading error";
		}

		BITMAPFILEHEADER ih; 
		BITMAPINFO fi; 

		if(strcmp(filetype, "-i") == 0){
			ih = readFileHeader(ofile);
			fi = readFileInfo(ofile);
			fseek(ofile, ih.bfOffBits, SEEK_SET);
		}

		std::vector<uint8_t> data = getData(ofile);
		std::vector<uint8_t> result = cast256(data, action, key);

		if(strcmp(filetype, "-t") == 0){
			std::cout << "write text";
			writeData(rfile, result);
		}

		std::cout << filetype;

		if(strcmp(filetype, "-i") == 0){
			std::cout << "write image";
			writeData(rfile, result, fi, ih);
		}

		double d1 = getDistribution(data);
		double d2 = getDistribution(result);
		double corell = countCorell(data, result);

		std::cout << "\ninput 1: "<< d1 << ", 0: "<< 1 - d1 << std::endl;
		std::cout << "output 1: "<< d2 << ", 0: "<< 1 - d2 << std::endl;
		std::cout << "corell: " << corell << std::endl;

		fclose(ofile);
		fclose(rfile);

	} catch (char const* str){
		std::cout << str << std::endl;
	}
	return 0;
	
}
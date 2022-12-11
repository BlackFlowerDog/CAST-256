#include "rwfile.h"

std::vector<uint8_t> getData(FILE* file){
	std::vector<uint8_t> result;
	uint8_t c;
	while(!feof(file)){
		fread(&c,sizeof(c),1,file);
		if(!feof(file)){
			result.push_back(c);
		}
	}
	return result;
}

void writeData(FILE* file, std::vector<uint8_t>& data){
	for(int i = 0; i < data.size(); i++){
		fwrite(&data[i],sizeof(uint8_t),1,file);
	}
}
void writeData(FILE* file, std::vector<uint8_t>& data, BITMAPINFO fi, BITMAPFILEHEADER fh){
	std::fwrite(&fh, sizeof(BITMAPFILEHEADER), 1, file);
	std::fwrite(&fi, sizeof(BITMAPINFO),1,file);
	fseek(file, fh.bfOffBits, SEEK_SET);
	for(int i = 0; i < data.size(); i++){
		fwrite(&data[i],sizeof(uint8_t),1,file);
	}
}

BITMAPFILEHEADER readFileHeader(FILE* file){
	BITMAPFILEHEADER fh;
	fread(&fh,sizeof(BITMAPFILEHEADER),1,file);
	if(fh.bfType != BMP){
        throw "image is not BMP"
	}
	return fh;
}

BITMAPINFO readFileInfo(FILE* file){
	BITMAPINFO fi;
	fread(&fi,sizeof(BITMAPINFO),1,file);
	return fi;
}
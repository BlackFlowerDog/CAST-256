#include"cast256.h"
#include<fstream>
#include<iostream>
#include<cstring>
#include<random>

//получение блока 128 из массива
void getBlock(uint8_t* buffer128, std::vector<uint8_t>& data, int index){
	bool param = false;
	int i = 0;
	for(; i < 16; i++){
		if(data.size() <= i + index){
			param = true;
		}
		if(!param){
			buffer128[i] = data[index + i];
		} else {
			buffer128[i] = ' ';
		}
	}
}
//запись блока в массив
void writeBlock(std::vector<uint8_t>& data, uint8_t* buffer128){
	for(int i = 0; i < 16; i++){
		data.push_back(buffer128[i]);
	}
}
//управляющая функция (разбиение данных на блоки 128, on/off CFB, encrypt, decrypt -?)
std::vector<uint8_t> cast256(std::vector<uint8_t>& data, char* action, char* CFB){
	std::vector<uint8_t> result;
	uint8_t buffer128[16];
	uint8_t bufferres[16];
	uint32_t*** subkeysKrKm = create3Xarray(2,4,12);
	uint32_t* key = initKey(subkeysKrKm);

	uint8_t vectorInit[16] = {'a', 'c', 'd', '7', '7', 'q', 'x', 'z', 'f', 's', '1', '7', '0', '1', '4', '0'};
	applyKey(vectorInit, subkeysKrKm, vectorInit);
	
	if(strcmp(action, "-d") == 0 && strcmp(CFB, "-n") == 0){
		std::cout << "start decrypt\n";
		uint32_t bufferSubkeysKrKm[2][4][12];
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 12; j++){
				bufferSubkeysKrKm[0][i][j] = subkeysKrKm[0][i][j];
				bufferSubkeysKrKm[1][i][j] = subkeysKrKm[1][i][j];
			}
		}
		for (int i = 0; i < 4; i++){
        	for (int j = 0; j < 12; j++){
            	subkeysKrKm[0][i][j] = bufferSubkeysKrKm[0][i][11-j];
            	subkeysKrKm[1][i][j] = bufferSubkeysKrKm[1][i][11-j];
        	}
   	    }
	} 

	if(strcmp(CFB, "-n") == 0){
		for(int i = 0; i < data.size(); i = i + 16){
			getBlock(buffer128, data, i);
			applyKey(buffer128, subkeysKrKm, bufferres);
			writeBlock(result, bufferres);
		}
	} else if(strcmp(CFB, "-y") == 0){
		getBlock(buffer128, data, 0);
		xOr(buffer128, vectorInit, bufferres);
		writeBlock(result, bufferres);
		if(strcmp(action, "-e") == 0){
			for(int i = 16; i < data.size(); i = i + 16){
				getBlock(buffer128, data, i);
				applyKey(bufferres, subkeysKrKm, bufferres);
				xOr(buffer128, bufferres, bufferres);
				writeBlock(result, bufferres);	
			}
		} else if(strcmp(action, "-d") == 0){
			std::cout << "start CFB decrypt";
			for (int i = 16; i < data.size(); i = i + 16) {
				applyKey(buffer128, subkeysKrKm, bufferres);
				getBlock(buffer128, data, i);
				xOr(buffer128, bufferres, bufferres);
				writeBlock(result, bufferres);
			}
		}
 	} else {
 		throw std::runtime_error("CFB key are not correct");
 	}

	delete[] key;
	free(subkeysKrKm, 2, 4);
	return result;
}

void xOr(uint8_t* a, uint8_t* b, uint8_t* res){
	for(int i = 0; i < 16; i++){
		res[i] = a[i] ^ b[i];
	}
}

//шифрование
void applyKey(uint8_t* data, uint32_t*** KrKm, uint8_t* res){
	uint32_t data_t[4];
	for(int i = 0; i < 4; i++){
		data_t[i] = (uint32_t)data[4*i] << 24 | (uint32_t)data[1+4*i] << 16 | (uint32_t)data[4*i+2] << 8 | (uint32_t)data[4*i+3];
	}
	for(int i = 0; i < 6; i++){
		Q(data_t, i, KrKm);
	}
	for(int i = 6; i < 12; i++){
		nQ(data_t, i, KrKm);
	}
	for(int i = 0; i < 4; i++){
		res[4*i] = (data_t[i]&0xFF000000)>>24;
		res[4*i+1] = (data_t[i]&0x00FF0000)>>16;
		res[4*i+2] = (data_t[i]&0x0000FF00)>>8;
		res[4*i+3] = (data_t[i]&0x000000FF);
	}
}

//Q-функции
void Q(uint32_t* data, int round, uint32_t*** KrKm){
	data[2] = data[2] ^ f1(data[3], KrKm[0][0][round], KrKm[1][0][round]);
	data[1] = data[1] ^ f2(data[2], KrKm[0][1][round], KrKm[1][1][round]);
	data[0] = data[0] ^ f3(data[1], KrKm[0][2][round], KrKm[1][2][round]);
	data[3] = data[3] ^ f1(data[0], KrKm[0][3][round], KrKm[1][3][round]);
}
void nQ(uint32_t* data, int round, uint32_t*** KrKm){
	data[3] = data[3] ^ f1(data[0], KrKm[0][3][round], KrKm[1][3][round]);
	data[0] = data[0] ^ f3(data[1], KrKm[0][2][round], KrKm[1][2][round]);
	data[1] = data[1] ^ f2(data[2], KrKm[0][1][round], KrKm[1][1][round]);
	data[2] = data[2] ^ f1(data[3], KrKm[0][0][round], KrKm[1][0][round]);
}

//раундовые функции

uint32_t f1(uint32_t data, uint32_t Tr, uint32_t Tm){
	uint32_t I = (data + Tm);
	I = (I << Tr) | (I >> (32 - Tr));
	uint8_t Ia = (I&0xFF000000)>>24;
	uint8_t Ib = (I&0x00FF0000)>>16;
	uint8_t Ic = (I&0x0000FF00)>>8;
	uint8_t Id = (I&0x000000FF);
	return ((S1[Ia] ^ S2[Ib])-S3[Ic])+S4[Id];
}

uint32_t f2(uint32_t data, uint32_t Tr, uint32_t Tm){
	uint32_t I = data ^ Tm;
	I = (I << Tr) | (I >> (32 - Tr));
	uint8_t Ia = (I&0xFF000000)>>24;
	uint8_t Ib = (I&0x00FF0000)>>16;
	uint8_t Ic = (I&0x0000FF00)>>8;
	uint8_t Id = (I&0x000000FF);
	return 	(S1[Ia] - S2[Ib] + S3[Ic]) ^ S4[Id];
}

uint32_t f3(uint32_t data, uint32_t Tr, uint32_t Tm){
	uint32_t I = (data - Tm);
	I = (I << Tr) | (I >> (32 - Tr));
	uint8_t Ia = (I&0xFF000000)>>24;
	uint8_t Ib = (I&0x00FF0000)>>16; 
	uint8_t Ic = (I&0x0000FF00)>>8;
	uint8_t Id = (I&0x000000FF);
	return ((S1[Ia] + S2[Ib]) ^ S3[Ic]) - S4[Id];
}

//функиция автоматической генерации ключа
uint32_t* keyGenerate(int size){
	uint32_t* key = new uint32_t[8];
	if(!(size == 16 || size == 20 || size == 24 || size == 28 || size == 32)){
		throw std::runtime_error("size of key are not correct (16, 20, 24, 28 or 32 symbols");
	}
	for(int i = 0; i < 8; i++){
		uint32_t x;
		if(i < size/4){
			x = (uint32_t)rand()/RAND_MAX;
		} else{
			x = 0;
		}
		key[i] = x;
	}
	return key;
}

//инициализация ключа и подключей
//файл, в который должен быть записан ключ - key.txt

uint32_t* initKey(uint32_t*** subkeysKrKm){
	FILE* file = fopen("key.txt", "rb");
	uint32_t* key = new uint32_t[8];
	int i = 0;
	uint32_t c = 0;
	uint8_t d;
	while(!feof(file)){
		fread(&d,sizeof(d),1,file);
		if(!feof(file)){
			i++;
		}
	}
	if(!(i == 16 || i == 20 || i == 24 || i == 28 || i == 32)){
		std::cout << i;
		throw std::runtime_error("size of key are not correct (16, 20, 24, 28 or 32 symbols");
	}
	fseek(file, 0, SEEK_SET);
	i = 0;
	for(; i < 8; i++){
		fread(&c, sizeof(uint32_t),1,file);
		if(!feof(file)){
			key[i] = c;
		} else {
			break;
		}
	}
	for(; i < 8; i++){
		key[i] = 0;
	}
	
	fclose(file);

	uint32_t*** TrTm = initTrTm();

	for(int i = 0; i < 12; i++){
		w(2*i, key, TrTm);
		w(2*i+1, key, TrTm);
		KrKm(i, key, subkeysKrKm);
	}

	free(TrTm, 2, 8);
	return key;
}

void w(int round, uint32_t* key, uint32_t*** TrTm){
	key[6] = key[6] ^ f1(key[7], TrTm[0][0][round], TrTm[1][0][round]);
	key[5] = key[5] ^ f2(key[6], TrTm[0][1][round], TrTm[1][1][round]);
	key[4] = key[4] ^ f3(key[5], TrTm[0][2][round], TrTm[1][2][round]);
	key[3] = key[3] ^ f1(key[4], TrTm[0][3][round], TrTm[1][3][round]);
	key[2] = key[2] ^ f2(key[3], TrTm[0][4][round], TrTm[1][4][round]);
	key[1] = key[1] ^ f3(key[2], TrTm[0][5][round], TrTm[1][5][round]);
	key[0] = key[0] ^ f1(key[1], TrTm[0][6][round], TrTm[1][6][round]);
	key[7] = key[7] ^ f2(key[0], TrTm[0][7][round], TrTm[1][7][round]);
	std::cout << key[7] << "___";
}

//генерация подключей шифрования основных данных
void KrKm(int round, uint32_t* key, uint32_t*** KrKm){
	KrKm[0][0][round] = key[0]&0x1F;
	KrKm[0][1][round] = key[2]&0x1F;
	KrKm[0][2][round] = key[4]&0x1F;
	KrKm[0][3][round] = key[6]&0x1F;

	KrKm[1][0][round] = key[7];
	KrKm[1][1][round] = key[5];
	KrKm[1][2][round] = key[3];
	KrKm[1][3][round] = key[1];
}


//генерация подключей ключа
uint32_t*** initTrTm(){
	uint32_t*** TrTm = create3Xarray(2, 8, 24);
	uint32_t Cm = 0x5A827999;
	uint32_t Mm = 0x6ED9EBA1;
	uint32_t Cr = 19;
	uint32_t Mr = 17;

	for(int i = 0; i < 24; i++){
		for(int j = 0; j < 8; j++){
			TrTm[0][j][i] = Cr;
			TrTm[1][j][i] = Cm;
			//std::cout << TrTm[0][j][i];
			Cm = (Cm + Mm);
			Cr = (Cr + Mr)%32;	
		}
	}
	return TrTm;
}
//работа с 3-х мерными массивами подключей
uint32_t*** create3Xarray (int m, int n, int l){
    uint32_t*** arr  = new uint32_t**[m];
    for(int i = 0 ; i < m; ++i) {
        arr[i] = new uint32_t*[n];
        for(int j = 0; j < n; ++j) {
            arr[i][j] = new uint32_t[l];
            for(int k = 0; k< l; ++k){
                arr[i][j][k] = 0;
            }
        }
    }
	return arr;
}

void free(uint32_t*** arr, int m, int n){
    for(int i = 0; i < m; ++i){
        for(int j = 0; j < n; ++j){
            delete[] arr[i][j];
        }
        delete[] arr[i];
    }
    delete[] arr;
}
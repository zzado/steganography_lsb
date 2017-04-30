#include<stdio.h>
#include<stdlib.h> 

unsigned char* func(unsigned char* addr, int x, int y, int width, int height);

int main(){
	
	FILE* stego = fopen("./Stegano/alice_stego.bmp", "rb");
	
	// get file size
	size_t file_size;
	fseek (stego , 0 , SEEK_END);
    file_size = ftell (stego);
    rewind (stego);
    
	// read file
	unsigned char* buf;
	buf = (unsigned char *)malloc(file_size);
	fread(buf, 1, file_size, stego);
	
	// get width, height
	unsigned char* header = buf;	// header_length : 54byte
	int width=0;
	int height=0;
	
	for(int i=0; i<4; i++){
		width += (int)(*(header + 18 + i)) << (i * 8);
		height += (int)(*(header + 22 + i)) << (i * 8);
	}	
	printf("File size : %d / %d X %d\n", file_size, width, height);
	
	// get LSB
	unsigned char* data = buf+54;
	unsigned char* LSB = (unsigned char *)malloc(file_size-54);
	unsigned char* temp_ptr;
	int index=0;
	
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			temp_ptr = func(data, j, i, width, height);
			LSB[index] = *(temp_ptr+2) & 1;		// Red
			index++;
			LSB[index] = *(temp_ptr+1) & 1;		// Gren
			index++;
			LSB[index] = *(temp_ptr) & 1;		// Blue
			index++;
		}
	}
	
	// extract file
	FILE* result = fopen("./res.bmp","wb");
	for(int i=0; i<file_size-54; i+=8){
		unsigned char temp_char = 0x00;
		for(int j=0; j<8; j++){
			temp_char += LSB[i+j] << (7-j);	// binary(8 char) to byte(char)
		}
		fwrite(&temp_char, 1, 1, result);
	}	
}

unsigned char* func(unsigned char* addr, int x, int y, int width, int height){
	y = (height-1) -y;
	return &addr[ width*y*3 + 3*x ];		// ¿ÞÂÊ À§ºÎÅÍ (0, 0) 
}


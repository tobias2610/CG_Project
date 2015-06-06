#pragma once
#include "Object.h"
class Overlay
{
private:
	char text[100];
	Image image;
	void loadPic(char* path){
		// load an image
		unsigned char* image0 = stbi_load(path, &image.width, &image.height, &image.comp, 0);
		//flip image vertically
		image.image = (unsigned char*)malloc(sizeof(unsigned char)*image.width*image.height*image.comp);
		for (int y = 0, stride = image.width*image.comp; y < image.height; y++) memcpy(image.image + (image.height - 1 - y)*stride, image0 + y*stride, stride); // vertical flip
		stbi_image_free(image0); // release the original image
	}
public:
	Overlay(){

	}
	Overlay(char* pImage){
		loadPic(pImage);
	}
	char* getText(){
		return text;
	}
	unsigned char* getImage(){
		return image.image;
	}
	int getImageWidth(){
		return image.width;
	}
	int getImageHeight(){
		return image.height;
	}
	void setText(char* pText){
		
	}
	~Overlay(){

	}
};


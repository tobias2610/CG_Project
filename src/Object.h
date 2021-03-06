#include "cgmath.h"
// forward declaration of image loader function available at stb_image.c
extern "C" unsigned char*	stbi_load(const char* filename, int* x, int* y, int* comp, int req_comp);
extern "C" void				stbi_image_free(void* retval_from_stbi_load);
#pragma once

struct Image{
	unsigned char* image;
	int width;
	int height;
	int comp;
};

class Object
{
private:
	vec4 position;
	float scale;
	Image image;
	Mesh* mesh;

	void loadPic(char* path){
		// load an image
		unsigned char* image0 = stbi_load(path, &image.width, &image.height, &image.comp, 0);
		//flip image vertically
		image.image = (unsigned char*)malloc(sizeof(unsigned char)*image.width*image.height*image.comp);
		for (int y = 0, stride = image.width*image.comp; y < image.height; y++) memcpy(image.image + (image.height - 1 - y)*stride, image0 + y*stride, stride); // vertical flip
		stbi_image_free(image0); // release the original image
	}

public:
	Object(){

	}
	Object(float pScale, vec3 pPosition,char* pImagePath, char* pPath)
	{
		scale = pScale;
		position = pPosition;
		loadPic(pImagePath);
		if (strcmp(pPath, "Circle")==0){
			mesh = loadCircle();
		}
		else if (strcmp(pPath,"Box")==0){
			mesh = loadBox();
		}
		else if (strcmp(pPath, "Wall")==0){
			mesh = loadWall();
		}
		else{
			mesh = loadMesh(pPath);
		}
	}
	~Object(){

	}
	virtual Mesh* getMesh(){
		return mesh;
	}
	virtual unsigned char* getImage(){
		return image.image;
	}
	virtual int getImageWidth(){
		return image.width;
	}
	virtual int getImageHeight(){
		return image.height;
	}
	virtual vec4 getPosition(){
		return position;
	}
	float getScale(){
		return scale;
	}
	virtual void setPosition(vec4 pPos){
		position = pPos;
	}
};


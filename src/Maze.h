#pragma once
#include "cgmath.h"
#include "Object.h"

class Maze: public Object
{
private:
	vec3 direction;
	
public:
	Maze(){

	}
	Maze(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{
		direction = vec3(0, 0, 1);
		
	}
	
	vec3 getDirection(){
		return direction;
	}
	void setDirection(vec3 pDirection){
		direction = pDirection;
	}

	void shoot(){

	}



	~Maze()
	{
		//free(image);
	}
};


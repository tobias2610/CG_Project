#include "Object.h"
#include "cgmath.h"
#pragma once
class Wall : public Object
{
public:
	Wall(){

	}
	Wall(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{

	}
	bool clisionDetect(vec3 direct){

	}
	mat4 calcModelMatrix(vec3 position, vec3 rotationAxis, float angle, float scale){
		mat4 modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(scale, scale, scale) * modelMatrix;
		modelMatrix = mat4::translate(getPosition().x + position.x, getPosition().y + position.y, getPosition().z + position.z) * modelMatrix;
		modelMatrix = mat4::rotate(rotationAxis, angle);
		
		return modelMatrix;
	}
	
	~Wall()
	{
	}
};


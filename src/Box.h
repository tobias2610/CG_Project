#include "Object.h"
#include "cgmath.h"
#pragma once
class Box : public Object
{
public:
	Box(){

	}
	Box(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{
	}
	bool clisionDetect(){
		return false;
	}
	mat4 calcModelMatrix(vec3 position, vec3 rotationAxis, float angle){
		mat4 modelMatrix = mat4::identity();

		modelMatrix = mat4::scale(getScale(), getScale(), getScale()) * modelMatrix;
		if (angle != 0){
			modelMatrix = mat4::rotate(rotationAxis, angle);
		}
		modelMatrix = mat4::translate(getPosition().x + position.x, getPosition().y + position.y, getPosition().z + position.z) * modelMatrix;
		return modelMatrix;
	}
	~Box()
	{
	}
};


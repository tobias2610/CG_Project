#pragma once
#include "Object.h"
class Enemy : public Object
{
private:
	int HP;
public:

	Enemy()
	{
	}
	Enemy(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{

		HP = 100;
	}

	bool clisionDetect(){
		//if (world.getYRotation() == PI || world.getYRotation() == 2*PI)
		if (vec4(0,0,1,0).dot(getPosition())==0)
			return true;
		else
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
	void setHp(){
		setPosition(vec4(-2.f, -2.f, -20.f, 0.f));
		
	}
	int getHp(){
		return HP;
	}
	~Enemy()
	{
	}
};


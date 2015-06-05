#include "Object.h"
#include "cgmath.h"
#include "Wall.h"
#include "Object.h"
#pragma once
class World : public Object
{
private:
	float angleX;
	float angleY;
	std::map<char*,Object> surroundings;
	Wall wall;
	std::vector<vec4> positions;
	std::vector<mat4> wallModelMat;
	

public:
	World(){

	}
	World(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{//position is middle of the world
		

	}

	bool clisionDetect(vec3 direct){

	}
	float getXRotation(){
		return angleX;
	}

	float getYRotation(){
		return angleY;
	}

	void setXRotation(float dx){
		angleX = dx;
	}
	void setYRotation(float dy){
		angleY = dy;
	}
	void setWorldWalls(Wall simplewall){
		wall = simplewall;


		//front
		mat4 modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
		//	modelMatrix = mat4::rotate(vec3(0, 1, 0), (1 / 2.0 * PI));
		modelMatrix = mat4::translate(wall.getPosition().x, wall.getPosition().y, wall.getPosition().z) * modelMatrix;

		wallModelMat[0] = modelMatrix;

		//back
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), (PI));
		modelMatrix = mat4::translate(wall.getPosition().x, wall.getPosition().y, wall.getPosition().z + 10) * modelMatrix;

		wallModelMat[1] = modelMatrix;

		//left and right wall
		//right
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), (0.5 * PI));
		modelMatrix = mat4::translate(wall.getPosition().x + 5, wall.getPosition().y, wall.getPosition().z + 5) * modelMatrix;

		wallModelMat[2] = modelMatrix;


		//left
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), -(0.5 * PI));
		modelMatrix = mat4::translate(wall.getPosition().x - 5, wall.getPosition().y, wall.getPosition().z + 5) * modelMatrix;

		wallModelMat[3] = modelMatrix;



		//bottom and seiling

		//bottom
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), (0.5 * PI));
		modelMatrix = mat4::translate(wall.getPosition().x, wall.getPosition().y + 5, wall.getPosition().z + 5) * modelMatrix;

		wallModelMat[4] = modelMatrix;


		//seiling
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), -(0.5 * PI));
		modelMatrix = mat4::translate(wall.getPosition().x, wall.getPosition().y - 5, wall.getPosition().z + 5) * modelMatrix;

		wallModelMat[5] = modelMatrix;
	}
	std::vector<mat4> getWorldWalls(){
		
		return wallModelMat;
	}

	~World()
	{
	}
};


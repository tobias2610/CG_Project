#include "Object.h"
#include "cgmath.h"
#include "Wall.h"
#include "Object.h"
#include "Enemy.h"
#include "AK.h"
#pragma once
class World : public Object
{
private:
	float angleX;
	float angleY;
	const int NUM_WORLDWALLS = 6;
	//list of modelMatrix for each Object Cathegory
	std::vector<mat4> surroundings;
	std::vector<mat4> worldWalls;
	std::vector<mat4> enemies;
	Wall wall;
	Enemy enemy;
	Wall worldWall;
	AK ak;
	
	

public:

	World(Wall sWall, Enemy sEnemy, Wall sworldWall, AK sAk)
	{

		wall = sWall;
		enemy = sEnemy;
		worldWall = sworldWall;
		ak = sAk;

	}

	World(){

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
	int getNum_WorldWalls(){
		return NUM_WORLDWALLS;
	}
	Wall getWall(){
		return worldWall;
	}
	void setAK(AK simAK){
		ak = simAK;
	}
	AK getAK(){
		return ak;
	}
	void setWorldWalls(Wall simplewall){
		worldWall = simplewall;
		worldWalls.resize(6 * sizeof(mat4));


		//front
		//mat4 modelMatrix = mat4::identity();
		//modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		////	modelMatrix = mat4::rotate(vec3(0, 1, 0), (1 / 2.0 * PI));
		//modelMatrix = mat4::translate(worldWall.getPosition().x, worldWall.getPosition().y, worldWall.getPosition().z) * modelMatrix;
		vec3 position = vec3(0,0,0);
		vec3 rotationAxis = vec3(0, 1, 0);
		float angle = 0.5 * PI;
		mat4 modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);

		worldWalls[0] = modelMatrix;

		//back
		/*modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), (PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x, worldWall.getPosition().y, worldWall.getPosition().z + 10) * modelMatrix;
*/
		position = vec3(0, 0, 10);
		//rotationAxis = vec3(0, 1, 0);
		angle = PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);

		worldWalls[1] = modelMatrix;

		//left and right wall
		//right
		/*modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), (0.5 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x + 5, worldWall.getPosition().y, worldWall.getPosition().z + 5) * modelMatrix;
*/
		position = vec3(5, 0, 5);
		//rotationAxis = vec3(0, 1, 0);
		angle = 0.5 * PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);

		//worldWalls[1] = modelMatrix;

//		worldWalls[2] = modelMatrix;


		//left
		/*modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), -(0.5 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x - 5, worldWall.getPosition().y, worldWall.getPosition().z + 5) * modelMatrix;*/

		position = vec3(-5, 0, 5);
		//rotationAxis = vec3(0, 1, 0);
		angle = -0.5 * PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);

		worldWalls[3] = modelMatrix;



		//bottom and seiling

		//bottom
		/*modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), (0.5 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x, worldWall.getPosition().y + 5, worldWall.getPosition().z + 5) * modelMatrix;*/

		position = vec3(0, 5, 5);
		rotationAxis = vec3(1, 0, 0);
		angle = 0.5 * PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);

		worldWalls[4] = modelMatrix;


		//seiling
		/*modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), -(0.5 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x, worldWall.getPosition().y - 5, worldWall.getPosition().z + 5) * modelMatrix;*/

		position = vec3(0, -5, 5);
		rotationAxis = vec3(1, 0, 0);
		angle = -0.5 * PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);

		worldWalls[5] = modelMatrix;
	}
	void initEnemies(){

	}
	void initSurroundingWalls(){

	}
	std::vector<mat4> getWorldWalls(){
		
		return worldWalls;
	}

	~World()
	{
	}
};


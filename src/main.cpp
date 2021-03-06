#include <windows.h> 
#include "time.h"
#include "support.h"
#include "cgmath.h"		// slee's simple math library
#include "trackball.h"
#include "mesh.h"
#include "light.h"
#include "AK.h"
#include "Box.h"
#include "Enemy.h"
#include "Wall.h"
#include "world.h"
#include "Maze.h"
#include "Aim.h"
#include "Overlay.h"
#define stepSize 0.5f
#define num_boxes 4
#define num_enemyes 17
#define num_Walls 6
time_t timer;
irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
static int menu_id;
static int submenu_id;
static int value = 0;
//*******************************************************************
// index variables for OpenGL objects
GLuint	program = 0;					// ID holder for GPU program
GLuint	vertexPositionBuffer = 0;		// ID holder for vertex position buffer
int     windowWidth, windowHeight;

//*******************************************************************
// global variables
int			frame = 0;	// index of rendering frames
bool		bMouseLButtonDown = false;
Camera		camera;
AK			ak;
Aim			aim;
Box			box[num_boxes];
Wall		wallBrown;
Wall		worldWall;
Enemy		enemy[num_enemyes];
Maze		maze;
Trackball	trackball(camera.viewMatrix, 1.0f);
GLuint		textureObject = 0;
Light		light;
Material	material;
Overlay		text;
//std::vector<Wall> Map;
//std::vector<Enemy> Enemys;
int xBefore = 0;
int yBefore = 0;
World  *world;

//*******************************************************************

void drawString(char *string){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();             //save
	glLoadIdentity();           //and clear
	gluOrtho2D(0, 1024, 0, 720);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1, 1, 1); // red font

	glDisable(GL_DEPTH_TEST); //disable depth test so renders on top

	glRasterPos2i(10, 10);
	void *font = GLUT_BITMAP_HELVETICA_18;
	for (char* c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}

	glEnable(GL_DEPTH_TEST);     //turn depth test back on

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


void update()
{
	// configure projection matrix
	camera.fovy = PI / 6.0f;
	//camera.aspectRatio; // udpate in reshape()
	camera.dNear = 1.0f;
	camera.dFar = 1000.0f;
	camera.projectionMatrix = mat4::perspective(camera.fovy, camera.aspectRatio, camera.dNear, camera.dFar);

	// setup uniform matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, camera.viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, camera.projectionMatrix);
	glBindTexture(GL_TEXTURE_2D, textureObject);
	glUniform1i(glGetUniformLocation(program, "TEX"), 0);	 // GL_TEXTURE0

	// setup light properties
	glUniform4fv(glGetUniformLocation(program, "lightPosition"), 1, light.position);
	glUniform4fv(glGetUniformLocation(program, "Ia"), 1, light.ambient);
	glUniform4fv(glGetUniformLocation(program, "Id"), 1, light.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Is"), 1, light.specular);

	// setup material properties
	glUniform4fv(glGetUniformLocation(program, "Ka"), 1, material.ambient);
	glUniform4fv(glGetUniformLocation(program, "Kd"), 1, material.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Ks"), 1, material.specular);
	glUniform1f(glGetUniformLocation(program, "shininess"), material.shininess);
	glUniform1i(glGetUniformLocation(program, "shininessT"), 1);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// notify to GL that we like to use our program now
	glUseProgram(program);


	////*****************************************Maze******************************************

	//// bind vertex position buffer
	//glBindBuffer(GL_ARRAY_BUFFER, maze.getMesh()->vertexBuffer);

	//// bind vertex position buffer
	//GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
	//glEnableVertexAttribArray(vertexPositionLoc);
	//glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	//// bind vertex normal buffer
	//GLuint vertexNormalLoc = glGetAttribLocation(program, "normal");
	//glEnableVertexAttribArray(vertexNormalLoc);
	//glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	//// bind vertex texture buffer
	//GLuint vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	//glEnableVertexAttribArray(vertexTexlLoc);
	//glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	//glTexImage2D(GL_TEXTURE_2D, 0, 3, maze.getImageWidth(), maze.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, maze.getImage());

	//for (int k = 1, w = maze.getImageWidth() >> 1, h = maze.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
	//	glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	//glGenerateMipmap(GL_TEXTURE_2D);

	//mat4 modelMatrix = mat4::identity();
	//modelMatrix = mat4::scale(maze.getScale(), maze.getScale(), maze.getScale()) * modelMatrix;
	//modelMatrix = mat4::translate(maze.getPosition().x + world->getPosition().x, maze.getPosition().y + world->getPosition().y, maze.getPosition().z + world->getPosition().z) * modelMatrix;
	//modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	//modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;
	//

	//glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	//glDrawArrays(GL_TRIANGLES, 0, maze.getMesh()->vertexList.size());

	//

	//*****************************************************World-Walls*********************************************************
	glBindBuffer(GL_ARRAY_BUFFER, worldWall.getMesh()->vertexBuffer);
	// bind vertex position buffer
	GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	GLuint vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	GLuint vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, worldWall.getImageWidth(), worldWall.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, worldWall.getImage());

	for (int k = 1, w = worldWall.getImageWidth() >> 1, h = worldWall.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	//int numWalls = world->getNum_WorldWalls();
	//std::vector<mat4> worldWalls = world->getWorldWalls();


	//*********************front wall**********************
	mat4 modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
	modelMatrix = mat4::translate(0, 25, 0) * modelMatrix;
	modelMatrix = mat4::translate(worldWall.getPosition().x + world->getPosition().x, worldWall.getPosition().y + world->getPosition().y, worldWall.getPosition().z + world->getPosition().z) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, worldWall.getMesh()->vertexList.size());

	//*********************right wall**********************
	modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
	//modelMatrix = mat4::translate(0, 3, 0) * modelMatrix;
	modelMatrix = mat4::translate(-25, 0, 0) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), (PI / 2)) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), -(PI / 2)) * modelMatrix;
	modelMatrix = mat4::translate(worldWall.getPosition().x + world->getPosition().x, worldWall.getPosition().y + world->getPosition().y, worldWall.getPosition().z + world->getPosition().z) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, worldWall.getMesh()->vertexList.size());

	//********************left wall****************************

	modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
	//modelMatrix = mat4::translate(0, 3, 0) * modelMatrix;
	modelMatrix = mat4::translate(25, 0, 0) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), -(PI / 2)) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), -(PI / 2)) * modelMatrix;
	modelMatrix = mat4::translate(worldWall.getPosition().x + world->getPosition().x, worldWall.getPosition().y + world->getPosition().y, worldWall.getPosition().z + world->getPosition().z) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, worldWall.getMesh()->vertexList.size());


	////********************top bottom****************************

	modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
	//modelMatrix = mat4::translate(0, 3, 0) * modelMatrix;
	modelMatrix = mat4::translate(0, 0, -25) * modelMatrix;
	//modelMatrix = mat4::rotate(vec3(0, 1, 0), PI ) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), (PI / 2)) * modelMatrix;
	modelMatrix = mat4::translate(worldWall.getPosition().x + world->getPosition().x, worldWall.getPosition().y + world->getPosition().y, worldWall.getPosition().z + world->getPosition().z) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, worldWall.getMesh()->vertexList.size());


	////********************top wall****************************
	modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
	//modelMatrix = mat4::translate(0, 3, 0) * modelMatrix;
	modelMatrix = mat4::translate(0, 0, 25) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), PI) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), (PI / 2)) * modelMatrix;
	modelMatrix = mat4::translate(worldWall.getPosition().x + world->getPosition().x, worldWall.getPosition().y + world->getPosition().y, worldWall.getPosition().z + world->getPosition().z) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, worldWall.getMesh()->vertexList.size());



	////********************back wall****************************
	modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
	modelMatrix = mat4::translate(0, 25, 0) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), PI) * modelMatrix;

	modelMatrix = mat4::translate(worldWall.getPosition().x + world->getPosition().x, worldWall.getPosition().y + world->getPosition().y, worldWall.getPosition().z + world->getPosition().z) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, worldWall.getMesh()->vertexList.size());

	////************************************************Brown-Walls****************************************************
	glBindBuffer(GL_ARRAY_BUFFER, wallBrown.getMesh()->vertexBuffer);
	// bind vertex position buffer
	vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, wallBrown.getImageWidth(), wallBrown.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, wallBrown.getImage());

	for (int k = 1, w = wallBrown.getImageWidth() >> 1, h = wallBrown.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(wallBrown.getScale(), wallBrown.getScale() / 2, wallBrown.getScale()) * modelMatrix;
	modelMatrix = mat4::translate(10, -10, -20) * modelMatrix;
	modelMatrix = mat4::translate(wallBrown.getPosition().x + world->getPosition().x, wallBrown.getPosition().y + world->getPosition().y, wallBrown.getPosition().z + world->getPosition().z) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, wallBrown.getMesh()->vertexList.size());


	modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(wallBrown.getScale(), wallBrown.getScale() / 2, wallBrown.getScale()) * modelMatrix;
	modelMatrix = mat4::translate(-20, -10, -50) * modelMatrix;
	modelMatrix = mat4::translate(wallBrown.getPosition().x + world->getPosition().x, wallBrown.getPosition().y + world->getPosition().y, wallBrown.getPosition().z + world->getPosition().z) * modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, wallBrown.getMesh()->vertexList.size());

	//*****************************************************BOX*********************************************************
	glBindBuffer(GL_ARRAY_BUFFER, box[0].getMesh()->vertexBuffer);

	// bind vertex position buffer
	vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, box[0].getImageWidth(), box[0].getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, box[0].getImage());

	for (int k = 1, w = box[0].getImageWidth() >> 1, h = box[0].getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	for (int i = 0; i < 1; i++){
		if (box[i].getPosition().z<0)
			box[i].setPosition(box[i].getPosition() + vec4(0, 0, 1.f, 0.f)*0.003f*(float)frame);
		else{
			box[i].setPosition(vec4(1 + (float)i, 0, -200, 0));
			ak.damage(5);
		}
		mat4 modelMatrix = mat4::identity();
		//	modelMatrix = mat4::translate(box[i].getPosition().x + world->getPosition().x);
		modelMatrix = mat4::scale(box[i].getScale(), box[i].getScale(), box[i].getScale()) * modelMatrix;
		modelMatrix = mat4::translate(box[i].getPosition().x + world->getPosition().x, box[i].getPosition().y + world->getPosition().y, box[i].getPosition().z + world->getPosition().z) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;

		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

		glDrawArrays(GL_TRIANGLES, 0, box[i].getMesh()->vertexList.size());
	}

	//*****************************************************ENEMY**********************************************************************************

	glBindBuffer(GL_ARRAY_BUFFER, enemy[0].getMesh()->vertexBuffer);

	// bind vertex position buffer
	vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, enemy[0].getImageWidth(), enemy[0].getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, enemy[0].getImage());

	for (int k = 1, w = enemy[0].getImageWidth() >> 1, h = enemy[0].getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);


	for (int i = 0; i < num_enemyes; i++){
		if (enemy[i].getHp()>0){
			mat4 modelMatrix = mat4::identity();
			modelMatrix = mat4::scale(4, 16, enemy[i].getScale()) * modelMatrix;
			modelMatrix = mat4::rotate(vec3(0, 1, 0), (PI / 2))*modelMatrix;
			modelMatrix = mat4::translate(enemy[i].getPosition().x + world->getPosition().x, enemy[i].getPosition().y + world->getPosition().y, enemy[i].getPosition().z - 5 + world->getPosition().z) * modelMatrix;
			modelMatrix = mat4::rotate(vec3(0, 1, 0), world->getXRotation())*modelMatrix;
			modelMatrix = mat4::rotate(vec3(1, 0, 0), world->getYRotation())*modelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

			glDrawArrays(GL_TRIANGLES, 0, enemy[i].getMesh()->vertexList.size());
		}
	}

	//*****************************************************aim**********************************************************************************
	glBindBuffer(GL_ARRAY_BUFFER, ak.getAim()->getMesh()->vertexBuffer);

	// bind vertex position buffer
	vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, ak.getAim()->getImageWidth(), ak.getAim()->getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, ak.getAim()->getImage());

	for (int k = 1, w = ak.getAim()->getImageWidth() >> 1, h = ak.getAim()->getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	modelMatrix = mat4::identity();
	modelMatrix = mat4::scale(ak.getAim()->getScale(), ak.getAim()->getScale(), ak.getAim()->getScale()) * modelMatrix;
	modelMatrix = mat4::translate(ak.getAim()->getPosition().x, ak.getAim()->getPosition().y, ak.getAim()->getPosition().z) * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	glDrawArrays(GL_TRIANGLES, 0, ak.getAim()->getMesh()->vertexList.size());



	//*******************************************AK***********************************************************************

	// bind vertex position buffer
	glBindBuffer(GL_ARRAY_BUFFER, ak.getMesh()->vertexBuffer);

	// bind vertex position buffer
	vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, ak.getImageWidth(), ak.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, ak.getImage());

	for (int k = 1, w = ak.getImageWidth() >> 1, h = ak.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	modelMatrix = mat4::identity();
	modelMatrix = mat4::rotate(vec3(0, 0, 1), PI)*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), -PI / 2)*modelMatrix;
	modelMatrix = mat4::translate(-camera.at) * modelMatrix;
	modelMatrix = mat4::scale(ak.getScale(), ak.getScale(), ak.getScale()) * modelMatrix;
	modelMatrix = mat4::translate(ak.getPosition().x, ak.getPosition().y, ak.getPosition().z) * modelMatrix;


	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	glDrawArrays(GL_TRIANGLES, 0, ak.getMesh()->vertexList.size());
	glUniform1i(glGetUniformLocation(program, "shininessT"), 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, ak.getImageWidth(), ak.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, text.getImage());

	for (int k = 1, w = ak.getImageWidth() >> 1, h = ak.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	char Overlay[100];
	sprintf(Overlay, "HP: %d              score: %d                 %d/%d", ak.getHP(), ak.getScore(), ak.getBullets(), ak.getBulletStock());
	drawString(Overlay);
	// now swap backbuffer with front buffer, and display it
	glutSwapBuffers();
	// increment FRAME index
	frame++;


}


void display()
{
	update();
	render();

}

void reshape(int width, int height)
{
	// set current viewport in pixels
	// viewport: the window area that are affected by rendering
	// (win_x, win_y, win_width, win_height)
	glViewport(0, 0, windowWidth = width, windowHeight = height);

	camera.aspectRatio = float(width) / float(height);

	// post signal to call display
	// this causes GL to call display() soon
	// but, we do not know exactly when dipslay() is called
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if (ak.getBullets() > 0){
			ak.setBullets(ak.getBullets() - 1);
			engine->play2D("../bin/Sounds/Shoot.wav");
			for (int i = 0; i < num_enemyes; i++){
				if (enemy[i].clisionDetect(world->getPosition(), world->getXRotation(), world->getYRotation())){
					ak.setScore(10);
					enemy[i].setHp();
				}
			}
			for (int j = 0; j < num_boxes; j++){
				if (box[j].clisionDetect(world->getPosition(), world->getXRotation(), world->getYRotation())){
					ak.setScore(20);
					box[j].setPosition(vec4(1 + (float)j, 0, -200, 0));
				}

			}
		}
		else{
			//munition leer nachladen!!!!
			engine->play2D("../bin/Sounds/empty.wav");
		}


	}
}

void motion(int x, int y)
{
	static bool just_warped = false;

	if (just_warped) {
		just_warped = false;
		return;
	}

	int dx = x - windowWidth / 2;
	int dy = y - windowHeight / 2;

	if (dx) {
		world->setXRotation((float)dx / 1000 + world->getXRotation());
	}

	if (dy) {
		world->setYRotation((float)dy / 1000 + world->getYRotation());
	}
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	just_warped = true;
	//world->setWorldWalls(world->getWall());
}

void move(int key, int x, int y)
{

}

void idle()
{
	glutPostRedisplay();	// signal to call display soon
}

void keyboard(unsigned char key, int x, int y)
{
	time_t timeT;
	vec4 a = mat4(cos((2 * PI - world->getXRotation())), 0, (sin((2 * PI - world->getXRotation()))), 0.f, 0, 1, 0.f, 0.f, sin((2 * PI - world->getXRotation())), 0.f, cos((2 * PI - world->getXRotation())), 0.f, 0.f, 0.f, 0.f, 1.f).operator*(vec4(0, 0, 1, 0));
	vec4 b = mat4(cos((world->getXRotation())), 0, (sin((world->getXRotation()))), 0.f, 0, 1, 0.f, 0.f, sin((world->getXRotation())), 0.f, cos((world->getXRotation())), 0.f, 0.f, 0.f, 0.f, 1.f).operator*(vec4(1, 0, 0, 0));
	printf("%f", world->getPosition().x + a.x*stepSize);
	if ((world->getPosition().x + a.x*stepSize > -85 && world->getPosition().x + a.x*stepSize < 0 && (world->getPosition().z + a.z*stepSize) < 85) && (world->getPosition().z + a.z*stepSize) > 0){
		if (world->getPosition().x + a.x*stepSize > -8 || world->getPosition().z + a.z*stepSize>12.5 || world->getPosition().z + a.z*stepSize < 11.5){
			if (world->getPosition().x + a.x*stepSize < -77 || world->getPosition().z + a.z*stepSize > 40.5 || world->getPosition().z + a.z*stepSize < 39.5){
				if ((key == 'w' || key == 'W')){
					time(&timeT);
					if (difftime(timeT, timer) >= 14){
						engine->play2D("../bin/Sounds/walk.wav");
						time(&timer);
					}
					world->setPosition(world->getPosition() + a*stepSize);
					//world->setWorldWalls(world->getWall());
				}
			}
		}
	}
	if ((world->getPosition().x - a.x*stepSize > -85 && world->getPosition().x - a.x*stepSize < 0 && (world->getPosition().z - a.z*stepSize) < 85) && (world->getPosition().z - a.z*stepSize) > 0){
		if (world->getPosition().x - b.x*stepSize > -8 || world->getPosition().z - b.z*stepSize > 12.5 || world->getPosition().z - b.z*stepSize < 11.5){
			if (world->getPosition().x - a.x*stepSize < -77 || world->getPosition().z - a.z*stepSize > 40.5 || world->getPosition().z - a.z*stepSize < 39.5){
				if (key == 's' || key == 'S'){
					time(&timeT);
					if (difftime(timeT, timer) >= 14){
						engine->play2D("../bin/Sounds/walk.wav");
						time(&timer);
					}
					world->setPosition(world->getPosition() - a*stepSize);
					//world->setWorldWalls(world->getWall());
				}
			}
		}
	}
	if ((world->getPosition().x + b.x*stepSize > -85 && world->getPosition().x + b.x*stepSize < 0 && (world->getPosition().z + b.z*stepSize) < 85) && (world->getPosition().z + b.z*stepSize) > 0){
		if (world->getPosition().x + a.x*stepSize > -8 || world->getPosition().z + a.z*stepSize > 12.5 || world->getPosition().z + a.z*stepSize < 11.5){
			if (world->getPosition().x + b.x*stepSize < -77 || world->getPosition().z + b.z*stepSize > 40.5 || world->getPosition().z + b.z*stepSize < 39.5){
				if (key == 'a' || key == 'A'){
					time(&timeT);
					if (difftime(timeT, timer) >= 14){
						engine->play2D("../bin/Sounds/walk.wav");
						time(&timer);
					}
					world->setPosition(world->getPosition() + b*stepSize);
					//world->setWorldWalls(world->getWall());
				}
			}
		}
	}
	if ((world->getPosition().x - b.x*stepSize > -85 && world->getPosition().x - b.x*stepSize < 0 && (world->getPosition().z - b.z*stepSize) < 85) && (world->getPosition().z - b.z*stepSize) > 0){
		if (world->getPosition().x - b.x*stepSize > -8 || world->getPosition().z - b.z*stepSize > 12.5 || world->getPosition().z - b.z*stepSize < 11.5){
			if (world->getPosition().x - b.x*stepSize < -77 || world->getPosition().z - b.z*stepSize > 40.5 || world->getPosition().z - b.z*stepSize < 39.5){
				if (key == 'd' || key == 'D'){
					time(&timeT);
					if (difftime(timeT, timer) >= 14){
						engine->play2D("../bin/Sounds/walk.wav");
						time(&timer);
					}
					world->setPosition(world->getPosition() - b*stepSize);
					//world->setWorldWalls(world->getWall());
				}
			}
		}
	}
	if (key == 'r' || key == 'R'){
		if (ak.getBulletStock() > 0){
			if (ak.getBulletStock() >= ak.getMaxBullet()){
				ak.setBullets(ak.getMaxBullet());
				ak.setBulletStock(ak.getBulletStock() - ak.getMaxBullet());
			}
			else{
				ak.setBullets(ak.getBulletStock());
				ak.setBulletStock(0);
			}
			engine->play2D("../bin/Sounds/reload.wav");
		}
		else{
			//keine munition mehr
			engine->play2D("../bin/Sounds/empty.wav");
		}

	}
	else if (key == 27){
		exit(0);
	}
}

bool initShaders(const char* vertShaderPath, const char* fragShaderPath)
{
	// create a program before linking shaders
	program = glCreateProgram();
	glUseProgram(program);

	// compile shader sources
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = readShader(vertShaderPath); if (vertexShaderSource == NULL) return false;
	GLint vertexShaderLength = strlen(vertexShaderSource);
	glShaderSource(vertexShader, 1, &vertexShaderSource, &vertexShaderLength);
	glCompileShader(vertexShader);
	if (!checkShader(vertexShader, "vertexShader")){ printf("Unable to compile vertex shader\n"); return false; }
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = readShader(fragShaderPath); if (fragmentShaderSource == NULL) return false;
	GLint fragmentShaderLength = strlen(fragmentShaderSource);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, &fragmentShaderLength);
	glCompileShader(fragmentShader);
	if (!checkShader(fragmentShader, "fragmentShader")){ printf("Unable to compile fragment shader\n"); return false; }
	// attach vertex/fragments shaders and link program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	if (!checkProgram(program, "program")){ printf("Unable to link program\n"); return false; }

	// deallocate string
	free((void*)vertexShaderSource);
	free((void*)fragmentShaderSource);

	return true;
}

bool userInit()
{
	time(&timer - 30);
	if (!engine)
	{
		printf("Could not startup engine\n");
		return 0; // error starting up the engine
	}
	wallBrown = Wall(100.f, vec3(48.f, 20.f, -48.f), "../bin/Images/hindranceWall.jpg", "Wall");
	worldWall = Wall(100.f, vec3(48.f, 15.f, -48.f), "../bin/Images/worldWall2.jpg", "Wall");
	//maze = Maze(1.f, vec3(0.f, 5.f, 0.f), "../bin/Images/wall_texture.jpg", "../bin/Mods/Maze.obj");

	ak = AK(0.006f, vec3(0, 0, 0), "../bin/Images/tex_AK.jpg", "../bin/Mods/AK.obj");
	text = Overlay("../bin/Images/Tex_1.jpg");


	box[0] = Box(0.5f, vec3(0.f, 0.f, -20.f), "../bin/Images/Box.jpg", "Box");
	box[1] = Box(0.5f, vec3(5.f, 0.f, -50.f), "../bin/Images/Box.jpg", "Box");
	box[2] = Box(0.5f, vec3(10.f, 0.f, -20.f), "../bin/Images/Box.jpg", "Box");

	enemy[0] = Enemy(0.1f, vec3(0.f, -5.f, -60.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[1] = Enemy(0.1f, vec3(60.f, -5.f, -70.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[3] = Enemy(0.1f, vec3(28.f, -5.f, -15.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[4] = Enemy(0.1f, vec3(14.f, -5.f, -50.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[5] = Enemy(0.1f, vec3(30.f, -5.f, -45.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[6] = Enemy(0.1f, vec3(65.f, -5.f, -20.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[7] = Enemy(0.1f, vec3(70.f, -5.f, -8.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[8] = Enemy(0.1f, vec3(50.f, -5.f, -60.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[9] = Enemy(0.1f, vec3(10.f, -5.f, -20.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[10] = Enemy(0.1f, vec3(40.f, -5.f, -10.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[11] = Enemy(0.1f, vec3(30.f, -5.f, -34.f), "../bin/Images/Enemy.jpg", "Box");
	enemy[12] = Enemy(0.1f, vec3(25.f, -5.f, -40.f), "../bin/Images/Enemy.jpg", "Box");

	world = new World();
	// create a vertex buffer
	/*glGenBuffers(1, &maze.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, maze.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, maze.getMesh()->vertexList.size()*sizeof(vertex), &maze.getMesh()->vertexList[0], GL_STATIC_DRAW);
	*/
	glGenBuffers(1, &worldWall.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, worldWall.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, worldWall.getMesh()->vertexList.size()*sizeof(vertex), &worldWall.getMesh()->vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &wallBrown.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, wallBrown.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, wallBrown.getMesh()->vertexList.size()*sizeof(vertex), &wallBrown.getMesh()->vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ak.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ak.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, ak.getMesh()->vertexList.size()*sizeof(vertex), &ak.getMesh()->vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ak.getAim()->getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ak.getAim()->getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, ak.getAim()->getMesh()->vertexList.size()*sizeof(vertex), &ak.getAim()->getMesh()->vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &box[0].getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, box[0].getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, box[0].getMesh()->vertexList.size()*sizeof(vertex), &box[0].getMesh()->vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &enemy[0].getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, enemy[0].getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, enemy[0].getMesh()->vertexList.size()*sizeof(vertex), &enemy[0].getMesh()->vertexList[0], GL_STATIC_DRAW);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// init camera
	camera.eye = vec3(0, 0.2f, 1);
	camera.at = vec3(0, 0, 0);
	camera.up = vec3(0, 1, 0);
	camera.viewMatrix = mat4::lookAt(camera.eye, camera.at, camera.up);

	// init light properties
	light.position = vec4(0.0f, 0.2f, 1.0f, 0.0f);   // directional light
	light.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	light.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// init material properties
	material.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	material.diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	material.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	material.shininess = 100.0f;

	return true;
}

void menu(int op) {

	switch (op) {
	case 1:
		userInit();
		break;
	case 0:
		exit(0);
	}
}

void createMenu(void){

	submenu_id = glutCreateMenu(menu);
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("New Try", 1);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}




int main(int argc, char* argv[])
{

	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	windowWidth = 1280;
	windowHeight = 720;

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
	glutCreateWindow("FPS");
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();
	glutWarpPointer(windowHeight / 2, windowWidth / 2);

	// Register callbacks
	createMenu();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);		// callback when the window is resized
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(move);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);		// callback when the mouse is moving
	glutIdleFunc(idle);			// idle-time callback

	// init and check GLEW, version, extensions
	if (!initExtensions()){ printf("Failed to init extensions.\n"); return 0; }
	// create and compile shaders/program
	if (!initShaders("shaders/trackball.vert", "shaders/trackball.frag")){ printf("Failed to init program and shaders\n"); return 0; }
	// user initialization
	if (!userInit()){ printf("Failed to userInit()\n"); return 0; }
	// Start rendering loop
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutMainLoop();
	return 0;
}
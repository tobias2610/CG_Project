#include "Object.h"
#include "cgmath.h"
class Box : public Object
{
public:
	Box(){

	}
	Box(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{
	}
	bool clisionDetect(vec4 pPos, float xRotation, float yRotation){

		mat4 modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(getScale(), getScale(), getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), (PI / 2))*modelMatrix;
		modelMatrix = mat4::translate(getPosition().x + pPos.x, getPosition().y + pPos.y, getPosition().z - 5 + pPos.z) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), xRotation)*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), yRotation)*modelMatrix;

		vec4 a = (modelMatrix*getPosition()).normalize();
		vec4 b = vec4(0, 0, 1, 0);
		vec4 c = vec4(0, 0, a.z, 0);
		float dz = acos(b.dot(a + c) / (b.length()*(a + c).length()));

		//if (world.getYRotation() == PI || world.getYRotation() == 2*PI)
		if ((a - vec4(0, 0, 0, 0) + b).y >= -0.001 && (a - vec4(0, 0, 0, 0) + b).y <= 0.1f && (a - vec4(0, 0, 0, 0) + b).x <= -0.9f && (a - vec4(0, 0, 0, 0) + b).x >= -1.0f && (dz) >= 1.5f && dz <= 2.5f)
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
	~Box()
	{
	}
};


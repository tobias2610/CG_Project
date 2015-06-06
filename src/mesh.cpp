#include "mesh.h"
#include "objloader.h"

Mesh* loadMesh(char* object )
{
	Mesh* pNewMesh = new Mesh();
	sp::ObjLoader loader = sp::ObjLoader();
	
	loader.load(object);
	size_t vertexBufferSize = loader.getVertCount();
	pNewMesh->vertexList.resize(vertexBufferSize*sizeof(vertex));
	const float* pos = loader.getPositions();
	const float* norm = loader.getNormals();
	const float* tex = loader.getTexCoords(100);
	int counterTex=0;
	int counterNorm=0;
	int counterPos=0;
	
	for (unsigned int i = 0; i < vertexBufferSize; i++){
		pNewMesh->vertexList[i].pos = vec3(pos[counterPos], pos[counterPos + 1], pos[counterPos + 2]);
		pNewMesh->vertexList[i].norm = vec3(norm[counterNorm], norm[counterNorm + 1], norm[counterNorm + 2]);
		pNewMesh->vertexList[i].tex = vec2(tex[counterTex], tex[counterTex + 1]);
		counterTex += 2;
		counterPos += 3;
		counterNorm += 3;
	}
	/*size_t indexBufferSize = loader.getIndexCount();
	pNewMesh->indexList.resize(indexBufferSize*sizeof(uint));
	for (int i = 0; i < indexBufferSize; i++){
		loader.get
	}*/


	return pNewMesh;
}

Mesh* loadBox(){
	float d = 0.5f;
	Mesh* pNewMesh = new Mesh();
	sp::ObjLoader loader = sp::ObjLoader();
	size_t vertexBufferSize = 6 * 6;
	pNewMesh->vertexList.resize(vertexBufferSize*sizeof(vertex));
	//front
	pNewMesh->vertexList[0].pos = vec3(d,d,d);
	pNewMesh->vertexList[1].pos = vec3(-d, d, d);
	pNewMesh->vertexList[2].pos = vec3(d, -d, d);

	pNewMesh->vertexList[3].pos = vec3(d, -d, d);
	pNewMesh->vertexList[4].pos = vec3(-d, d, d);
	pNewMesh->vertexList[5].pos = vec3(-d, -d, d);
	//back
	pNewMesh->vertexList[6].pos = vec3(d, d, -d);
	pNewMesh->vertexList[7].pos = vec3(-d, d, -d);
	pNewMesh->vertexList[8].pos = vec3(d, -d, -d);

	pNewMesh->vertexList[9].pos = vec3(d, -d, -d);
	pNewMesh->vertexList[10].pos = vec3(-d, d, -d);
	pNewMesh->vertexList[11].pos = vec3(-d, -d, -d);
	//top
	pNewMesh->vertexList[12].pos = vec3(d, d, d);
	pNewMesh->vertexList[13].pos = vec3(d, d, -d);
	pNewMesh->vertexList[14].pos = vec3(-d, d, -d);

	pNewMesh->vertexList[15].pos = vec3(d, d, d);
	pNewMesh->vertexList[16].pos = vec3(-d, d, -d);
	pNewMesh->vertexList[17].pos = vec3(-d, d, d);
	//bottom
	pNewMesh->vertexList[18].pos = vec3(d, -d, d);
	pNewMesh->vertexList[19].pos = vec3(d, -d, -d);
	pNewMesh->vertexList[20].pos = vec3(-d, -d, -d);

	pNewMesh->vertexList[21].pos = vec3(d, -d, d);
	pNewMesh->vertexList[22].pos = vec3(-d, -d, -d);
	pNewMesh->vertexList[23].pos = vec3(-d, -d, d);
	//left
	pNewMesh->vertexList[24].pos = vec3(-d, d, d);
	pNewMesh->vertexList[25].pos = vec3(-d, d, -d);
	pNewMesh->vertexList[26].pos = vec3(-d, -d, d);

	pNewMesh->vertexList[27].pos = vec3(-d, -d, d);
	pNewMesh->vertexList[28].pos = vec3(-d, d, -d);
	pNewMesh->vertexList[29].pos = vec3(-d, -d, -d);
	//right
	pNewMesh->vertexList[30].pos = vec3(d, d, d);
	pNewMesh->vertexList[31].pos = vec3(d, d, -d);
	pNewMesh->vertexList[32].pos = vec3(d, -d, d);

	pNewMesh->vertexList[33].pos = vec3(d, -d, d);
	pNewMesh->vertexList[34].pos = vec3(d, d, -d);
	pNewMesh->vertexList[35].pos = vec3(d, -d, -d);

	//normal
	//front
	pNewMesh->vertexList[0].norm = vec3(d, d, d).normalize();
	pNewMesh->vertexList[1].norm = vec3(-d, d, d).normalize();
	pNewMesh->vertexList[2].norm = vec3(d, -d, d).normalize();

	pNewMesh->vertexList[3].norm = vec3(d, -d, d).normalize();
	pNewMesh->vertexList[4].norm = vec3(-d, d, d).normalize();
	pNewMesh->vertexList[5].norm = vec3(-d, -d, d).normalize();
	//back
	pNewMesh->vertexList[6].norm = vec3(d, d, -d).normalize();
	pNewMesh->vertexList[7].norm = vec3(-d, d, -d).normalize();
	pNewMesh->vertexList[8].norm = vec3(d, -d, -d).normalize();

	pNewMesh->vertexList[9].norm = vec3(d, -d, -d).normalize();
	pNewMesh->vertexList[10].norm = vec3(-d, d, -d).normalize();
	pNewMesh->vertexList[11].norm = vec3(-d, -d, -d).normalize();
	//top
	pNewMesh->vertexList[12].norm = vec3(d, d, d).normalize();
	pNewMesh->vertexList[13].norm = vec3(d, d, -d).normalize();
	pNewMesh->vertexList[14].norm = vec3(-d, d, -d).normalize();

	pNewMesh->vertexList[15].norm = vec3(d, d, d).normalize();
	pNewMesh->vertexList[16].norm = vec3(-d, d, -d).normalize();
	pNewMesh->vertexList[17].norm = vec3(-d, d, d).normalize();
	//bottom
	pNewMesh->vertexList[18].norm = vec3(d, -d, d).normalize();
	pNewMesh->vertexList[19].norm = vec3(d, -d, -d).normalize();
	pNewMesh->vertexList[20].norm = vec3(-d, -d, -d).normalize();

	pNewMesh->vertexList[21].norm = vec3(d, -d, d).normalize();
	pNewMesh->vertexList[22].norm = vec3(-d, -d, -d).normalize();
	pNewMesh->vertexList[23].norm = vec3(-d, -d, d).normalize();
	//left
	pNewMesh->vertexList[24].norm = vec3(-d, d, d).normalize();
	pNewMesh->vertexList[25].norm = vec3(-d, d, -d).normalize();
	pNewMesh->vertexList[26].norm = vec3(-d, -d, d).normalize();

	pNewMesh->vertexList[27].norm = vec3(-d, -d, d).normalize();
	pNewMesh->vertexList[28].norm = vec3(-d, d, -d).normalize();
	pNewMesh->vertexList[29].norm = vec3(-d, -d, -d).normalize();
	//right
	pNewMesh->vertexList[30].norm = vec3(d, d, d).normalize();
	pNewMesh->vertexList[31].norm = vec3(d, d, -d).normalize();
	pNewMesh->vertexList[32].norm = vec3(d, -d, d).normalize();

	pNewMesh->vertexList[33].norm = vec3(d, -d, d).normalize();
	pNewMesh->vertexList[34].norm = vec3(d, d, -d).normalize();
	pNewMesh->vertexList[35].norm = vec3(d, -d, -d).normalize();

	//tex
	//front
	pNewMesh->vertexList[0].tex = vec2(1, 1);
	pNewMesh->vertexList[1].tex = vec2(0, 1);
	pNewMesh->vertexList[2].tex = vec2(1, 0);

	pNewMesh->vertexList[3].tex = vec2(1, 0);
	pNewMesh->vertexList[4].tex = vec2(0, 1);
	pNewMesh->vertexList[5].tex = vec2(0, 0);
	//back
	pNewMesh->vertexList[6].tex = vec2(1, 1);
	pNewMesh->vertexList[7].tex = vec2(0, 1);
	pNewMesh->vertexList[8].tex = vec2(1, 0);

	pNewMesh->vertexList[9].tex = vec2(1, 0);
	pNewMesh->vertexList[10].tex = vec2(0, 1);
	pNewMesh->vertexList[11].tex = vec2(0, 0);
	//top
	pNewMesh->vertexList[12].tex = vec2(1, 1);
	pNewMesh->vertexList[13].tex = vec2(0, 1);
	pNewMesh->vertexList[14].tex = vec2(1, 0);

	pNewMesh->vertexList[15].tex = vec2(1, 0);
	pNewMesh->vertexList[16].tex = vec2(0, 1);
	pNewMesh->vertexList[17].tex = vec2(0, 0);
	//bottom
	pNewMesh->vertexList[18].tex = vec2(1, 1);
	pNewMesh->vertexList[19].tex = vec2(0, 1);
	pNewMesh->vertexList[20].tex = vec2(1, 0);

	pNewMesh->vertexList[21].tex = vec2(1, 0);
	pNewMesh->vertexList[22].tex = vec2(0, 1);
	pNewMesh->vertexList[23].tex = vec2(0, 0);
	//left
	pNewMesh->vertexList[24].tex = vec2(1, 1);
	pNewMesh->vertexList[25].tex = vec2(0, 1);
	pNewMesh->vertexList[26].tex = vec2(1, 0);

	pNewMesh->vertexList[27].tex = vec2(1, 0);
	pNewMesh->vertexList[28].tex = vec2(0, 1);
	pNewMesh->vertexList[29].tex = vec2(0, 0);
	//right
	pNewMesh->vertexList[30].tex = vec2(1, 1);
	pNewMesh->vertexList[31].tex = vec2(0, 1);
	pNewMesh->vertexList[32].tex = vec2(1, 0);

	pNewMesh->vertexList[33].tex = vec2(1, 0);
	pNewMesh->vertexList[34].tex = vec2(0, 1);
	pNewMesh->vertexList[35].tex = vec2(0, 0);

	return pNewMesh;
}

Mesh* loadWall(){
	float d = 0.5f;
	Mesh* pNewMesh = new Mesh();
	sp::ObjLoader loader = sp::ObjLoader();
	size_t vertexBufferSize = 6;
	pNewMesh->vertexList.resize(vertexBufferSize*sizeof(vertex));
	//front
	pNewMesh->vertexList[0].pos = vec3(d, d, d);
	pNewMesh->vertexList[1].pos = vec3(-d, d, d);
	pNewMesh->vertexList[2].pos = vec3(d, -d, d);

	pNewMesh->vertexList[3].pos = vec3(d, -d, d);
	pNewMesh->vertexList[4].pos = vec3(-d, d, d);
	pNewMesh->vertexList[5].pos = vec3(-d, -d, d);

	//normal
	//front
	pNewMesh->vertexList[0].norm = vec3(d, d, d).normalize();
	pNewMesh->vertexList[1].norm = vec3(-d, d, d).normalize();
	pNewMesh->vertexList[2].norm = vec3(d, -d, d).normalize();

	pNewMesh->vertexList[3].norm = vec3(d, -d, d).normalize();
	pNewMesh->vertexList[4].norm = vec3(-d, d, d).normalize();
	pNewMesh->vertexList[5].norm = vec3(-d, -d, d).normalize();

	//tex
	//front
	pNewMesh->vertexList[0].tex = vec2(1, 1);
	pNewMesh->vertexList[1].tex = vec2(0, 1);
	pNewMesh->vertexList[2].tex = vec2(1, 0);

	pNewMesh->vertexList[3].tex = vec2(1, 0);
	pNewMesh->vertexList[4].tex = vec2(0, 1);
	pNewMesh->vertexList[5].tex = vec2(0, 0);

	return pNewMesh;
}

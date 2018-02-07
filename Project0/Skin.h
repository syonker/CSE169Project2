

#pragma once

#include "Core.h"
#include "Joint.h"
#include "Tokenizer.h"
#include <iostream>
#include "Vertex.h"
#include "Skeleton.h"

class Skin
{

public:

	Skin(Skeleton* skel);
	~Skin();

	bool Load(const char *file);
	bool LoadMorph(const char *file1, const char *file2);
	void Update(glm::mat4 parentW);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void BindShader();

	unsigned int numV;
	std::vector<Vertex*> vertices;

	std::vector<Vertex*> vertices2;
	std::vector<Vertex*> vertices3;

	Skeleton* skeleton;

	std::vector<float> shaderVerts;
	std::vector<unsigned int> shaderIndices;
	std::vector<float> shaderNormals;
	uint shader;
	uint VBO, VBO2, VAO, EBO;



	std::vector<float> shaderVerts2;
	std::vector<unsigned int> shaderIndices2;
	std::vector<float> shaderNormals2;



	std::vector<float> shaderVerts3;
	std::vector<unsigned int> shaderIndices3;
	std::vector<float> shaderNormals3;








	bool oldRender;

	uint VertexBuffer;
	uint NormalBuffer;
	uint IndexBuffer;


private:


};

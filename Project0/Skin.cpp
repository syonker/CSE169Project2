
#include "Skin.h"

Skin::Skin(Skeleton* skel) {

	skeleton = skel;

	BindShader();

}

Skin::~Skin() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);

}


void Skin::BindShader() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*shaderVerts.size(), shaderVerts.data(), GL_STATIC_DRAW);

	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*shaderNormals.size(), shaderNormals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);


	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*shaderIndices.size(), &shaderIndices[0], GL_STATIC_DRAW);


	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);


}



bool Skin::Load(const char *file) {

	if (file == "") {

		file = "../skeletons/wasp.skin";
	}

	std::cerr << "file: " << file << std::endl;

	Tokenizer* token = new Tokenizer();
	token->Open(file);

    //make vertices
	token->FindToken("positions");
	numV = token->GetFloat();

	//assign positions
	token->FindToken("{");

	float x, y, z;

	for (int count = 1; count <= numV; count++) {

		x = token->GetFloat();
		x = token->GetFloat();
		z = token->GetFloat();

		Vertex* newVertex = new Vertex(x,y,z);

		vertices.push_back(newVertex);

	}

	token->FindToken("}");

	//assign normals
	token->FindToken("normals");
	numV = token->GetFloat();
	token->FindToken("{");

	//float x, y, z;

	for (int count = 1; count <= numV; count++) {

		x = token->GetFloat();
		x = token->GetFloat();
		z = token->GetFloat();

		vertices[count - 1]->SetNormal(x,y,z);

	}

	token->FindToken("}");

	//assign attatchments
	token->FindToken("skinweights");
	numV = token->GetFloat();
	token->FindToken("{");

	float jointWeight;
	unsigned int numAttatchments, jointNum;

	for (int count = 1; count <= numV; count++) {

		numAttatchments = token->GetFloat();

		for (int aCount = 1; aCount <= numAttatchments; aCount++) {

			jointNum = token->GetFloat();
			jointWeight = token->GetFloat();

			vertices[count - 1]->joints.push_back(skeleton->joints[jointNum]);
			vertices[count - 1]->weights.push_back(jointWeight);

		}

	

	}

	token->FindToken("}");


	//save triangles
	token->FindToken("triangles");
	int numT = token->GetFloat();
	token->FindToken("{");

	for (int count = 1; count <= numT; count++) {

		shaderIndices.push_back(token->GetFloat());
		shaderIndices.push_back(token->GetFloat());
		shaderIndices.push_back(token->GetFloat());

	}

	token->FindToken("}");


	//save binding to joints (need skeleton)
	token->FindToken("bindings");
	int numJ = token->GetFloat();
	token->FindToken("{");

	for (int count = 1; count <= numJ; count++) {

		token->FindToken("matrix");
		token->FindToken("{");

		glm::mat4 B = 
			glm::mat4(glm::vec4(token->GetFloat(), token->GetFloat(), token->GetFloat(), 0.0f),
			glm::vec4(token->GetFloat(), token->GetFloat(), token->GetFloat(), 0.0f),
			glm::vec4(token->GetFloat(), token->GetFloat(), token->GetFloat(), 0.0f),
			glm::vec4(token->GetFloat(), token->GetFloat(), token->GetFloat(), 1.0f));

		skeleton->joints[count - 1]->Binv = glm::inverse(B);

		token->FindToken("}");

	}

	
	
	
	token->FindToken("}");

	//std::cerr << "Final Count: " << joints.size() << std::endl;


	// Finish
	token->Close();
	return true;

}

void Skin::Update(glm::mat4 parentW) {


}

void Skin::Draw(const glm::mat4 &viewProjMtx, uint shader) {

	//send things to shader

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)shaderIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


}
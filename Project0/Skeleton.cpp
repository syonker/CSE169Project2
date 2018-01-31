
#include "Skeleton.h"



Skeleton::Skeleton()
{

	root = NULL;

}

Skeleton::~Skeleton()
{
}


bool Skeleton::Load(const char *file) {
	
	if (file == "") {

		file = "../skeletons/test.skel";
	}


	Tokenizer* token = new Tokenizer();
	//token->Open(file, "skel");
	token->Open(file);
	token->FindToken("balljoint");
	// Parse tree
	root = new Joint();
	root->Load(token);


	makeJointVector();
	jointNum = 0;

	activeJoint = root;
	activeDOF = root->jointDOF[0];

	//std::cerr << "Final Count: " << joints.size() << std::endl;


	// Finish
	token->Close();
	return true;
}


void Skeleton::Update(glm::mat4 parentW) {

	root->Update(parentW);

}

void Skeleton::Draw(const glm::mat4 &viewProjMtx, uint shader) {

	root->Draw(viewProjMtx, shader);

}



void Skeleton::makeJointVector() {


	root->makeJointVector(&joints);

}

void Skeleton::upSelection() {

	Joint* currJoint = joints[jointNum];

	if (currJoint->DOFnum < currJoint->DOFcount) {

		//do the thing at DOFnum
		activeDOF = currJoint->jointDOF[currJoint->DOFnum];

		currJoint->DOFnum++;

	}
	else {

		if (jointNum < joints.size()-1) {

			jointNum++;

		}
		else {

			jointNum = 0;

		}
		

		joints[jointNum]->DOFnum = 0;


	}


}

void Skeleton::downSelection() {


	Joint* currJoint = joints[jointNum];

	if (currJoint->DOFnum > 0) {

		//do the thing at DOFnum
		activeDOF = currJoint->jointDOF[currJoint->DOFnum];

		currJoint->DOFnum--;

	}
	else {

		if (jointNum > 0) {

			jointNum--;

		}
		else {

			jointNum = joints.size()-1;

		}

		joints[jointNum]->DOFnum = 2;


	}

}
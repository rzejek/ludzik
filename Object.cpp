/*
 * Object.cpp
 *
 *  Created on: 18 kwi 2020
 *      Author: andr
 */

#include "Object.h"

void Object::Draw(GLuint modelLoc)
{
	GltfModel->Draw(modelLoc);
}

void Object::Tick(double time)
{
    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, Position);
    ModelMatrix = glm::rotate(ModelMatrix, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    ModelMatrix = glm::rotate(ModelMatrix, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    ModelMatrix = glm::rotate(ModelMatrix, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    ModelMatrix = glm::scale(ModelMatrix, Scale);

    GltfModel->Tick();

	map<int, Model *>::iterator iter_mod = GltfModel->ModelsMap.begin();
	while (iter_mod != GltfModel->ModelsMap.end())
	{
		Model * model = iter_mod->second;
		int nodeid = iter_mod->first;

		glm::mat4 modelMat = GltfModel->MatricesMap[nodeid];
		modelMat = ModelMatrix * modelMat;
		GltfModel->MatricesMap[nodeid] = modelMat;

		iter_mod++;
	}

    map<int, Animation *>::iterator iter = Animations.CurrentAnimations.begin();
    while (iter != Animations.CurrentAnimations.end())
    {
    	Animation * anim = iter->second;
    	int nodeid = iter->first;
    	if (anim->function == "rotation")
    	{
    		glm::vec4 rot = Animations.GetInterpolatedValue4(nodeid, time);

    		glm::mat4 modelMatrix;
    	    modelMatrix = glm::mat4(1.0f);
    	    modelMatrix = glm::rotate(modelMatrix, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
    	    modelMatrix = glm::rotate(modelMatrix, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
    	    modelMatrix = glm::rotate(modelMatrix, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

    		glm::mat4 modelMat = GltfModel->MatricesMap[nodeid];
    		GltfModel->MatricesMap[nodeid] = modelMat*modelMatrix;
    	}
    	iter++;
    }

//	Rotation.y += 0.001f;
}

Object::Object()
{
}

Object::Object(ParsedObject * parsed)
{
	GltfModel = new LoadGltfModel();
	GltfModel->init_vao(parsed->type);

	Scale = parsed->scale;
	Rotation = parsed->rotate;
	Position = parsed->position;
}

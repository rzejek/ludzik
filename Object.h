/*
 * Object.h
 *
 *  Created on: 18 kwi 2020
 *      Author: andr
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "LoadGltfModel.h"
#include "LoadModel.h"
#include "AnimationManager.h"
#include "ParsedObject.h"
#include <GL/glew.h>
#include <map>

class Object
{
public:
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation;

    glm::mat4 ModelMatrix;

    LoadGltfModel * GltfModel;

    AnimationManager Animations;

	void Draw(GLuint modelLoc);
	virtual void Tick(double time);

	Object();
	Object(ParsedObject * parsed);
};

#endif /* OBJECT_H_ */

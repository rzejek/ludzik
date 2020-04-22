/*
 * Scene.h
 *
 *  Created on: 13 kwi 2020
 *      Author: andr
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "LoadGltfModel.h"
#include <GL/glew.h>

class Scene
{
public:
	LoadGltfModel Hero;
	void Draw(GLuint modelLoc);
	Scene();
};

#endif /* SCENE_H_ */

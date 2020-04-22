/*
 * LoadGltfModel.h
 *
 *  Created on: 13 kwi 2020
 *      Author: andr
 */

#ifndef LOADGLTFMODEL_H_
#define LOADGLTFMODEL_H_

#include "GraphObject.h"
#include "GltfBase.h"
#include <GL/glew.h>
#include <vector>

#include "LoadModel.h"

class LoadGltfModel : public LoadModel
{
public:
	map<int, Model *> ModelsMap;
	map<int, glm::mat4 > MatricesMap;

	GltfBase Base;
	LoadGltfModel();

    void Draw(GLuint modelLoc);
    void Tick();

    int LoadFromFile(string plik);
    void init_vao(string model_obj);

};

#endif /* LOADGLTFMODEL_H_ */

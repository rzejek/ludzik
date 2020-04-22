/*
 * LoadObjModel.h
 *
 *  Created on: 17 kwi 2020
 *      Author: andr
 */

#ifndef LOADOBJMODEL_H_
#define LOADOBJMODEL_H_

#include "LoadModel.h"
#include "LoadMaterial.h"

class LoadObjModel: public LoadModel
{
public:
    LoadObjModel();

    LoadObjModel(string dir, string model_obj);
    void init(string dir, string model_obj);
    string ModelObjName;

    int LoadFromFile(string plik);

    string mtllib;
    string model_dir;

    virtual ~LoadObjModel();

    int modelElemCount;
    int modelVertCount;
    Vertex * bufVert;
    void wypelnijBufor();

    void Draw();

    void init_vao(string model_obj);
    void init_vao(string dir, string model_obj);

    LoadMaterial material;

protected:
private:
};

#endif /* LOADOBJMODEL_H_ */

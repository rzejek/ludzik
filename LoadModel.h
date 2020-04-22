#ifndef LOADMODEL_H
#define LOADMODEL_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#include "GraphObject.h"


class LoadModel
{
public:
    vector <Model *> Modele;

    GLuint vao;
    GLuint vbo;

    LoadModel();
    LoadModel(string dir, string model_obj);

    string ModelObjName;
    string model_dir;

    virtual int LoadFromFile(string plik) = 0;

    virtual ~LoadModel();

    int modelElemCount;
    int modelVertCount;
    Vertex * bufVert;
    void wypelnijBufor();

    virtual void Draw();

    void init_vao();
    virtual void init_vao(string model_obj) = 0;
    void init_vao(string dir, string model_obj);

    Model * retModel(string name);
    Model * removeModel(string name);

    void addModel(Model * model);

protected:
private:
};

#endif // LOADMODEL_H

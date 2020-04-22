
#include <GL/glew.h>

#include "LoadModel.h"

#include <fstream>
#include <sstream>


LoadModel::LoadModel()
{
    init_vao();
}

LoadModel::LoadModel(string dir, string model_obj)
{
    init_vao();

    ModelObjName = model_obj;
    init_vao(dir, model_obj);
}

LoadModel::~LoadModel()
{
    //dtor
}

void LoadModel::init_vao()
{
	glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vbo);


    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(3*sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(6*sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void LoadModel::init_vao(string dir, string model_obj)
{
	model_dir = dir;

	init_vao(model_obj);
}

void LoadModel::Draw()
{
    glBindVertexArray(vao);
    vector<Model *>::iterator itmod;
    itmod = Modele.begin();

    int elemCount = 0;
    while (itmod != Modele.end())
    {
        Model * model = *itmod;

        //glUniform3fv(KolorID, 1, (GLfloat *)&load_material.Kolory[model->material]);
        //glDrawElements(GL_TRIANGLES, model->retElemCount(), GL_UNSIGNED_INT, (void *)(elemCount*sizeof(int)));

        glDrawArrays(GL_TRIANGLES, elemCount, model->retBufCount());

        elemCount += model->retBufCount();
        itmod++;
    }

    glBindVertexArray(0);
}

void LoadModel::wypelnijBufor()
{
//    modelElemCount = 0;
    modelVertCount = 0;

    int vertCount = 0;
//    int elemCount = 0;

    vector<Model *>::iterator itmod;
    itmod = Modele.begin();
    while (itmod != Modele.end())
    {
        Model * model = *itmod;
//        elemCount = model->retElemCount();
        vertCount = model->retBufCount();

//        modelElemCount += elemCount;
        modelVertCount += vertCount;
        itmod++;
    }

//    if (load_model.Modele.size() == 0) return;

    bufVert = new Vertex[modelVertCount];
//    GLint *bufElem = new GLint[modelElemCount];

    itmod = Modele.begin();
    vertCount = 0;
//    elemCount = 0;
    while (itmod != Modele.end())
    {
        Model * model = *itmod;

        model->retBufVerticesOnly(bufVert + vertCount);
//        model->retBuf(bufVert + vertCount);

//        model->retBufElem(bufElem + elemCount);

        vertCount += model->retBufCount();
//        elemCount += model->retElemCount();

//        cout << "elemCount:"<<elemCount<<" vertCount:"<<vertCount<<endl;

        itmod++;
    }
/*
        for (int i = 0; i < modelVertCount; i++)
        {
            bufVert[i].r = kolor.r;
            bufVert[i].g = kolor.g;
            bufVert[i].b = kolor.b;
        }
*/
//    cout << "modelElemCount:"<<modelElemCount<<" modelVertCount:"<<modelVertCount<<endl;


    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, modelVertCount*sizeof(Vertex), bufVert, GL_STATIC_DRAW);
    //cout << "sizeof(bufor_do_testu):"<<sizeof(bufor_do_testu)<<endl;
    //glBufferData(GL_ARRAY_BUFFER, sizeof(bufor_do_testu), bufor_do_testu, GL_STATIC_DRAW);

//    cout << "bufor: " << "ile:" << vertCount<<" pierwszy:"<<bufVert[0]<<", "<<bufVert[1]<<", "<<bufVert[2]<<endl;
//    cout << " drugi:"<<bufVert[3]<<", "<<bufVert[4]<<", "<<bufVert[5]<<endl;
  //  cout << " trzeci:"<<bufVert[6]<<", "<<bufVert[7]<<", "<<bufVert[8]<<endl;
    //cout << " czwarty:"<<bufVert[9]<<", "<<bufVert[10]<<", "<<bufVert[11]<<endl;

//    glGenBuffers(1, &iboElem);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


    //cout << "Ilosc elementow = " << elemCount <<", sizeof(bufElem)= "<<sizeof(bufElem)<< endl;
  //  cout << "elem0 = " <<bufElem[0] << ", elem1 = " <<bufElem[1] << ", elem2 = "<<bufElem[2] << ", elem3 = "<<bufElem[3]<<endl;
//    cout << "elem4 = " <<bufElem[4] << ", elem5 = " <<bufElem[5] << ", elem6 = "<<bufElem[6] << ", elem7 = "<<bufElem[7]<<endl;

//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelElemCount*sizeof(GLint), bufElem, GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_do_testu), Indices_do_testu, GL_STATIC_DRAW);

//    delete [] bufElem;
//    delete [] bufVert;

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model * LoadModel::retModel(string name)
{
	for (int i = 0; i < Modele.size(); i++)
	{
		Model * model = Modele[i];

		if (model->nazwa == name) return model;
	}

	return NULL;
}

Model * LoadModel::removeModel(string name)
{
	for (int i = 0; i < Modele.size(); i++)
	{
		Model * model = Modele[i];

		cout << "nazwa: " << model->nazwa << endl;

		if (model->nazwa == name) {
			Modele.erase(Modele.begin() + i);

			wypelnijBufor();

			return model;
		}
	}

	return NULL;
}

void LoadModel::addModel(Model * model)
{
	Modele.push_back(model);

	wypelnijBufor();
}

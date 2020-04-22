/*
 * LoadObjModel.cpp
 *
 *  Created on: 17 kwi 2020
 *      Author: andr
 */
#include "LoadObjModel.h"

#include <GL/glew.h>
#include <fstream>
#include <sstream>


LoadObjModel::LoadObjModel()
{
	LoadModel::init_vao();
}

void LoadObjModel::init(string dir, string model_obj)
{
    ModelObjName = model_obj;
    init_vao(dir, model_obj);
}

LoadObjModel::LoadObjModel(string dir, string model_obj)
{
	LoadModel::init_vao();
	init(dir, model_obj);
}

LoadObjModel::~LoadObjModel()
{
}

void LoadObjModel::init_vao(string dir, string model_obj)
{
	model_dir = dir;

	init_vao(model_obj);
}

void LoadObjModel::init_vao(string model_obj)
{
//	init_vao();

    glBindVertexArray(vao);

//    LoadFromFile(model_dir+string("/")+model_obj);

    mtllib = model_obj+".mtl";

    material.LoadFromFile(model_dir+string("/")+mtllib);

    LoadFromFile(model_dir+string("/")+model_obj+".obj");


    //ustawienie koloru modela
    vector<Model *>::iterator itmod;
    itmod = Modele.begin();
    while (itmod != Modele.end())
    {
        Model * model = *itmod;

//        model->wyznaczAllVertNormal();
//        model->ustawKolorAllVert(material.Kolory[model->material]);

        itmod++;
    }
/*
    glGenBuffers(1, &vbo);
*/
//    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

/*
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(3*sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(6*sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
*/

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    wypelnijBufor();

    glBindVertexArray(0);
}

void LoadObjModel::Draw()
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

void LoadObjModel::wypelnijBufor()
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

        model->retBuf(bufVert + vertCount);

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

int LoadObjModel::LoadFromFile(string plik)
{
//    Plik = plik;

    ifstream strumien(plik.c_str());
    if (!strumien) {
        cerr << "Nie mozna otworzyc pliku modelowego: "<<plik<<endl;
        return -1;
    }

    Vertex temp_vert;
    Triangle temp_tr;
    Model * temp_mod;// = new Model();

    bool first = true;
    int vertexCount = 0;

    int normalCount = 0;
    glm::vec3 temp_norm;

    string linia;
    while (getline(strumien, linia))
    {
        stringstream sstr(linia);

        string wyraz;
        sstr >> wyraz;

        //cout << "wyraz: " << wyraz<<endl;
        //char znak;
        if (wyraz == "#") //komentarz - pomin do konca linii
            continue;
        if (wyraz == "")
            continue;

        if (wyraz == "mtllib")
        {
            sstr >> mtllib;
        }

        if (wyraz == "usemtl")
        {
            sstr >> temp_mod->material;
        }

        if (wyraz == "o") {
            string nnn;
            sstr >> nnn;

            temp_mod = new Model();

            temp_mod->nazwa = nnn;

            Modele.push_back(temp_mod);

            if (!first)
            {
//                cout << "nazwa z LoadFromFile: " << temp_mod->nazwa << endl;
            }
            else
                first = false;
        }

        if (wyraz == "v") {
            float x, y, z;
            sstr >> x >> y >> z;
            temp_vert.x = x;
            temp_vert.y = y;
            temp_vert.z = z;

            temp_mod->wierzcholki[vertexCount++] = temp_vert;
        }

        if (wyraz == "vn") {
            float x, y, z;
            sstr >> x >> y >> z;
            temp_norm.x = x;
            temp_norm.y = y;
            temp_norm.z = z;

            temp_mod->normalne[normalCount++] = temp_norm;
        }

        if (wyraz == "s") {
        }

        if (wyraz == "f") {
            unsigned int a, b, c;
            unsigned int an, bn, cn;
            unsigned int att, btt, ctt;
            att = btt = ctt = 0;

            char znak , znak2;
            //string temp;
            //sstr >> temp;
            sstr >> a;
            sstr >> znak;
            //sstr >> att;
            sstr >> znak2;
            sstr >> an;

            //cout << "czyta: a:" << a<<" znak:" <<znak<<" att:"<<att<<" znak2:"<<znak2<<" an:"<<an<<endl;

            sstr >> b;
            sstr >> znak;
            //sstr >> btt;
            sstr >> znak2;
            sstr >> bn;

            //cout << "czyta: b:" << b<<" znak:" <<znak<<" btt:"<<btt<<" znak2:"<<znak2<<" bn:"<<bn<<endl;

            sstr >> c;
            sstr >> znak;
            //sstr >> ctt;
            sstr >> znak2;
            sstr >> cn;

            //cout << "czyta: c:" << c<<" znak:" <<znak<<" ctt:"<<ctt<<" znak2:"<<znak2<<" cn:"<<cn<<endl;

            temp_tr.a = a - 1;
            temp_tr.b = b - 1;
            temp_tr.c = c - 1;

            temp_tr.normalna = an - 1;

            temp_tr.kolor = material.Kolory[temp_mod->material];

            temp_mod->trojkaty.push_back(temp_tr);
        }
    }

//    cout <<"wynik wczytywania: "<< *temp_mod << endl;

    if (first == false)
    {
//        Modele.push_back(temp_mod);
    }

	for (int i = 0; i < Modele.size(); i++)
	{
		Model * model = Modele[i];

		cout << "nazwa LoadFromFile2 : " << model->nazwa << endl;
	}

    return 0;
}

/*
 * LoadGltfModel.cpp
 *
 *  Created on: 13 kwi 2020
 *      Author: andr
 */

#include "LoadGltfModel.h"


LoadGltfModel::LoadGltfModel()
{
	model_dir = "models";
}

int LoadGltfModel::LoadFromFile(string plik)
{
	Base.init(plik);

	int count_mod = 0;
	int count_tr = 0;
	int count_vert = 0;

	for (int i = 0; i < Base.Nodes.size(); i++)
	{
		Node node = Base.Nodes[i];
		cout << "Nodes i: " << i << " node.MeshId: " << node.MeshId << " meshes.size: " << Base.Meshes.size()<<endl;

		if (node.MeshId == -1) continue;

		Mesh mesh = Base.Meshes[node.MeshId];

//		Mesh mesh = Base.Meshes[i];

		Model * model = new Model;

		for (int i = 0; i < mesh.Primitives.size(); i++)
		{
			Primitive prim = mesh.Primitives[i];

			Accessor accPosition = Base.Accessors[prim.Position];
			Accessor accNormal = Base.Accessors[prim.Normal];
			Accessor accIndices = Base.Accessors[prim.Indices];

			Material material = Base.Materials[prim.Material];

//			cout << "Position "<<prim.Position<<" size: "<< Base.Accessors[prim.Position].Floats.size() << endl;
//			cout << "Normal "<<prim.Normal<<" size: "<< Base.Accessors[prim.Normal].Floats.size() << endl;
//			cout << "Indices "<<prim.Indices<<" size: "<< Base.Accessors[prim.Indices].ShortInts.size() << endl;
//			cout << "Material size: "<< accMaterial.size() << endl;
			for (int i = 0; i < accIndices.ShortInts.size(); )
			{
				Vertex vert[3];
				int abc[3];
				for (int j = 0; j < 3; j++, i++)
				{
					unsigned short int idx = accIndices.ShortInts[i];

					abc[j] = idx;

					float pos1 = accPosition.Floats[idx*3];
					float pos2 = accPosition.Floats[idx*3 + 1];
					float pos3 = accPosition.Floats[idx*3 + 2];

					float n1 = accNormal.Floats[idx*3];
					float n2 = accNormal.Floats[idx*3 + 1];
					float n3 = accNormal.Floats[idx*3 + 2];

					vert[j].x = pos1;
					vert[j].y = pos2;
					vert[j].z = pos3;

//					cout << "vert idx: " << idx <<" x: " <<vert[j].x<<" y: " <<vert[j].y<<" z: " <<vert[j].z<<endl;

					vert[j].nx = n1;
					vert[j].ny = n2;
					vert[j].nz = n3;

					vert[j].kolor =  Kolor(material.r, material.g, material.b);
					model->Vertices.push_back(vert[j]);

					count_vert++;
				}

				Triangle triangle;
				triangle.a = abc[0];
				triangle.b = abc[1];
				triangle.c = abc[2];

				triangle.normalna = triangle.b;

//				cout << "tr a: " <<triangle.a<<" b: " <<triangle.b<<" c: " <<triangle.c<<endl;

				model->wierzcholki[triangle.a] = vert[0];
				model->wierzcholki[triangle.b] = vert[1];
				model->wierzcholki[triangle.c] = vert[2];

				glm::vec3 normal1(vert[0].nx, vert[0].ny, vert[0].nz);
				glm::vec3 normal2(vert[1].nx, vert[1].ny, vert[1].nz);
				glm::vec3 normal3(vert[2].nx, vert[2].ny, vert[2].nz);

				model->normalne[triangle.a] = normal1;
				model->normalne[triangle.b] = normal2;
				model->normalne[triangle.c] = normal3;

				model->trojkaty.push_back(triangle);
				count_tr++;
			}

//			cout << "Model kolor: " << material.r << ", "<< material.g << ", "<< material.b << endl;
			model->ustawKolorAllVert(Kolor(material.r, material.g, material.b));
//			model->wyznaczAllVertNormal();

//			cout << "Model : "<<plik<<" count: " << count_mod  << " count triangles: " << model->trojkaty.size()  << " count vert: "<<model->wierzcholki.size()<<endl;
		}

		Modele.push_back(model);
		ModelsMap[i] = model;

		count_mod++;
	}

	return count_mod;
}

void LoadGltfModel::Draw(GLuint modelLoc)
{
	glBindVertexArray(vao);

	map<int, Model *>::iterator itmod;
	itmod = ModelsMap.begin();

	int elemCount = 0;
	while (itmod != ModelsMap.end())
	{
		Model * model = itmod->second;
		int node = itmod->first;

	    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &MatricesMap[node][0][0]);

		glDrawArrays(GL_TRIANGLES, elemCount, model->retBufCount());

		elemCount += model->retBufCount();
		itmod++;
	}

	glBindVertexArray(0);
}

void LoadGltfModel::Tick()
{
	map<int, Model *>::iterator iter = ModelsMap.begin();
	while (iter != ModelsMap.end())
	{
		Model * model = iter->second;
		int node_num = iter->first;

		glm::mat4 ModelMatrix(1.0f);
	    glm::vec3 Scale = Base.Nodes[node_num].Scale;
	    glm::vec3 Rotation = Base.Nodes[node_num].Rotation;
	    glm::vec3 Position = Base.Nodes[node_num].Position;

	    ModelMatrix = glm::mat4(1.0f);
	    ModelMatrix = glm::translate(ModelMatrix, Position);
	    ModelMatrix = glm::rotate(ModelMatrix, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	    ModelMatrix = glm::rotate(ModelMatrix, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	    ModelMatrix = glm::rotate(ModelMatrix, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	    ModelMatrix = glm::scale(ModelMatrix, Scale);

		MatricesMap[node_num] = ModelMatrix; // glm::mat4(1.0f);
		iter++;
	}
}

void LoadGltfModel::init_vao(string model_obj)
{
	//	init_vao();

	glBindVertexArray(vao);

	string filename = model_dir+string("/")+model_obj+".gltf";
	cout << "Loading... " << filename << endl;

	LoadFromFile(filename);

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

		cout << "Modele count: " << Modele.size() << endl;

	    wypelnijBufor();

	    glBindVertexArray(0);

}

#include "GraphObject.h"
#include <cstring>

using namespace std;


int Model::retVertCount()
{
    return wierzcholki.size();
}

int Model::retElemCount()
{
    return 3*trojkaty.size();
}

glm::vec3 Model::wyznaczVertNormal(int abc, Vertex & vert_buf)
{
	int norm;
	float nx = 0.0, ny = 0.0, nz = 0.0;
	glm::vec3 vnorm;
	int count = 0;

    typedef vector<Triangle>::iterator Iter;
    Iter iter = trojkaty.begin();
    while (iter != trojkaty.end())
    {
        if (iter->a == abc || iter->b == abc || iter->c == abc) {
        	norm = iter->normalna;

			glm::vec3 vn = normalne[norm];

			nx+= vn.x;
			ny+= vn.y;
			nz+= vn.z;

        	count++;
        }

        iter++;
    }
    if (count)
    {
    	vnorm.x = nx / count;
    	vnorm.y = ny / count;
    	vnorm.z = nz / count;
    }

    vert_buf.nx = vnorm.x;
    vert_buf.ny = vnorm.y;
    vert_buf.nz = vnorm.z;

    return vnorm;
}

void Model::wyznaczAllVertNormal()
{
	int abc;

    typedef map<int, Vertex>::iterator Iter;
    Iter iter = wierzcholki.begin();
    while (iter != wierzcholki.end())
    {
        int abc = iter->first;
        Vertex & vert = iter->second;
        glm::vec3 wynik = wyznaczVertNormal(abc, vert);

//        cout << "abc:" << abc << ", vert:"<<vert << ", norm( x:" <<wynik.x<<" y:" <<wynik.y<<" z:"<<wynik.z<<")"<<endl;
        iter++;
    }
}

int Model::retBufVert(Vertex * bufor)
{
    int ile = 0;

    typedef map<int, Vertex>::iterator Iter;
    Iter iter = wierzcholki.begin();
    while (iter != wierzcholki.end())
    {
        int pos = iter->first;
        Vertex vert = iter->second;
/*
        bufor[ile++] = vert.x;
        bufor[ile++] = vert.y;
        bufor[ile++] = vert.z;

        bufor[ile++] = vert.r;
        bufor[ile++] = vert.g;
        bufor[ile++] = vert.b;
*/
        bufor[ile++] = vert;

        iter++;
    }
    return ile;
}

int Model::retBufElem(GLint * bufor)
{
    int ile = 0;
    typedef vector<Triangle>::iterator Iter;
    Iter iter = trojkaty.begin();
    while (iter != trojkaty.end())
    {
        bufor[ile++] = iter->a;
        bufor[ile++] = iter->b;
        bufor[ile++] = iter->c;

        iter++;
    }
    return ile;
}

int Model::retBufCount()
{
	bufCount = trojkaty.size()*3;
	return bufCount;
}

int Model::retBuf(Vertex * bufor)
{
	bufCount = 0;

    typedef vector<Triangle>::iterator Iter;
    Iter iter = trojkaty.begin();
    int i = 0;

    while (iter != trojkaty.end())
    {
    	Triangle & triangle = *iter;
    	int a,b,c;
    	a= triangle.a;
    	b= triangle.b;
    	c= triangle.c;

    	bufCount += 3;

    	bufor[i++] = wierzcholki[a];
    	bufor[i - 1].kolor = triangle.kolor;
    	bufor[i - 1].nx = normalne[triangle.normalna].x;
    	bufor[i - 1].ny = normalne[triangle.normalna].y;
    	bufor[i - 1].nz = normalne[triangle.normalna].z;

    	bufor[i++] = wierzcholki[b];
    	bufor[i - 1].kolor = triangle.kolor;
    	bufor[i - 1].nx = normalne[triangle.normalna].x;
    	bufor[i - 1].ny = normalne[triangle.normalna].y;
    	bufor[i - 1].nz = normalne[triangle.normalna].z;

    	bufor[i++] = wierzcholki[c];
    	bufor[i - 1].kolor = triangle.kolor;
    	bufor[i - 1].nx = normalne[triangle.normalna].x;
    	bufor[i - 1].ny = normalne[triangle.normalna].y;
    	bufor[i - 1].nz = normalne[triangle.normalna].z;

    	iter++;
    }

	return bufCount;
}

int Model::retBufVerticesOnly(Vertex * bufor)
{
	memcpy(bufor, &Vertices[0], Vertices.size()*sizeof(Vertex));
//	bufor = &Vertices[0];
}

void Model::ustawKolorAllVert(Kolor kol)
{
    typedef map<int, Vertex>::iterator Iter;
    Iter iter = wierzcholki.begin();
    while (iter != wierzcholki.end())
    {
        int pos = iter->first;
        Vertex & vert = iter->second;

        vert.kolor = kol;

        iter++;
    }

    for (int i = 0; i < trojkaty.size(); i++)
    {
    	Triangle & tr = trojkaty[i];
    	tr.kolor = kol;
    }
}

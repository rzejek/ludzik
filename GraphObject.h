#ifndef GRAPHOBJECT_H
#define GRAPHOBJECT_H

#include <string>
#include <vector>
#include <map>

#include <iostream>
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


struct Kolor
{
    float r, g, b;
    Kolor() : r(1.0), g(1.0), b(1.0)
    { }
    Kolor(float r, float g, float b)
    {
    	 this->r = r;
    	 this->g = g;
    	 this->b = b;
    }
};


struct Vertex
{
    float x, y, z;

    Kolor kolor;

    float nx, ny, nz;
};

struct Triangle
{
    int a, b, c;
    int normalna;

    Kolor kolor;
};

struct Model
{
    std::map<int, Vertex > wierzcholki;
    std::map<int, glm::vec3 > normalne;
    std::vector<Triangle > trojkaty;

    std::string nazwa;
    std::string material;

    int retVertCount();
    int retBufVert(Vertex * bufor);

    int retElemCount();
    int retBufElem(GLint * bufor);

    int bufCount;
    int retBufCount();
    int retBuf(Vertex * bufor);

    std::vector<Vertex > Vertices;
    int retBufVerticesOnly(Vertex * bufor);

    Kolor kolor;
    void ustawKolorAllVert(Kolor kolor);

    glm::vec3 wyznaczVertNormal(int abc, Vertex & vert_buf);

    void wyznaczAllVertNormal();
};

#endif // GRAPHOBJECT_H

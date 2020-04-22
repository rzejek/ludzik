#ifndef LOADSHADER_H
#define LOADSHADER_H

#include <GL/glew.h>
#include <string>
#include <map>

using namespace std;

class LoadShader
{
    map<GLuint, string> shadery;

public:
    GLuint Program;

    GLuint CreateProgram();

    void dodajShader(string plik, GLuint type);

    void CompileAllShaders();
    void LinkProgram();

    LoadShader();
    virtual ~LoadShader();

protected:
};

#endif // LOADSHADER_H

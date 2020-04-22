#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "LoadShader.h"

LoadShader::LoadShader()
{
//    Program = prog;
}

GLuint LoadShader::CreateProgram()
{
    Program = glCreateProgram();
    return Program;
}

void LoadShader::dodajShader(string plik, GLuint type)
{
    shadery[type] = plik;
}

LoadShader::~LoadShader()
{
    //dtor
}

string file_read(const char * plik)
{
    fstream fin(plik, fstream::in);
    if (!fin) return "";

    stringstream sstr;
    sstr << fin.rdbuf();

    return sstr.str();
}

void LoadShader::CompileAllShaders()
{
    typedef map<GLuint, string>::iterator Iter;
    Iter iter = shadery.begin();
    while (iter != shadery.end())
    {
        GLenum typ = iter->first;
        string plik = iter->second;

        string zawartosc = file_read(plik.c_str());

        if (zawartosc == "") {
            cerr << "zle wczytany plik shadera - " << plik << endl;
        } else {
            GLuint shd = glCreateShader(typ);
            const char * str = zawartosc.c_str();
            const GLchar ** source = &str;

            glShaderSource(shd, 1, source, NULL);
            glCompileShader(shd);

            GLint status = GL_FALSE;
            glGetShaderiv(shd, GL_COMPILE_STATUS, &status);
            if (status == GL_FALSE) {
                cerr << "blad przy kompilacji shadera:" << plik << endl;

                GLint infoLogLength;
                glGetShaderiv(shd, GL_INFO_LOG_LENGTH, &infoLogLength);

                GLchar *strInfoLog = new GLchar[infoLogLength + 1];
                glGetShaderInfoLog(shd, infoLogLength, NULL, strInfoLog);

                cerr << "Info log: " << strInfoLog << endl;
                delete [] strInfoLog;

                iter++;
                continue;
            }

            glAttachShader(Program, shd);
            cout << " shader typu: "<<typ<<". " << shd << "attached." <<endl;
        }

//        cout << "plik:" << plik <<", typu:"<< typ<<endl;

        iter++;
    }
}

void LoadShader::LinkProgram()
{
    glLinkProgram(Program);

    GLint status= GL_FALSE;
    glGetProgramiv(Program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        cerr << "Błąd przy linkowaniu programu." <<endl;

        GLint infoLogLength = 0;
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &infoLogLength);
        //cerr << "ilosc znakow:" << infoLogLength<<endl;


        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(Program, infoLogLength, NULL, strInfoLog);
        //fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        if (infoLogLength > 0)
            cerr << "Program Info Log:"<<strInfoLog<<endl;
        delete[] strInfoLog;
    }
}

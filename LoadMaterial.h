#ifndef LOADMATERIAL_H
#define LOADMATERIAL_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#include "GraphObject.h"


class LoadMaterial
{
public:

    map<string , Kolor> Kolory;

    int LoadFromFile(string plik);

    LoadMaterial();

protected:
private:
};

#endif // LOADMATERIAL_H

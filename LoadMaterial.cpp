#include "LoadMaterial.h"

#include <fstream>
#include <sstream>

LoadMaterial::LoadMaterial()
{
    //ctor
}

int LoadMaterial::LoadFromFile(string plik)
{
//    Plik = plik;

    ifstream strumien(plik.c_str());
    if (!strumien) {
        cerr << "Nie mozna otworzyc pliku z materialem: "<<plik<<endl;
        return -1;
    }

    bool first = true;
    int materialCount = 0;

    string newmtl;

    string linia;
    while (getline(strumien, linia))
    {
        stringstream sstr(linia);

        string wyraz;
        sstr >> wyraz;

        if (wyraz == "#") //komentarz - pomin do konca linii
            continue;
        if (wyraz == "")
            continue;

        if (wyraz == "newmtl") {
            sstr >> newmtl;
        }

        if (wyraz == "Kd") {
            Kolor kolor;
            float fr, fg, fb;
            sstr >> fr >> fg >> fb;
            kolor.r = fr;
            kolor.g = fg;
            kolor.b = fb;

            Kolory[newmtl] = kolor;
        }
    }


    return 0;
}

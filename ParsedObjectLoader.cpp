/*
 * ParsedObjectLoader.cpp
 *
 *  Created on: 19 kwi 2020
 *      Author: andr
 */

#include "ParsedObjectLoader.h"

using namespace std;

string file_read(std::string plik)
{
    fstream fin(plik, fstream::in);
    if (!fin) return "";

    stringstream sstr;
    sstr << fin.rdbuf();

    return sstr.str();
}

void ParsedObjectLoader::Parse(std::string file)
{
	string data = file_read(file);

	JsonTable * table = new JsonTable("", EJsonObject);
	int count = 0;
	table->parse(data, count);

	for (int i = 0; i < table->object_values.size(); i++)
	{
		JsonObject * obj = table->object_values[i];
		ParsedObject * parsed = new ParsedObject();
		parsed->Parse(obj);
		Objects.push_back(parsed);
	}

//	cout << data<< endl;
//	cout << "Parsed data size: "<<data.length()<< ", count: "<< count<< ", objects count: " <<Objects.size()<<endl;
}

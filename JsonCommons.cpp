/*
 * JsonCommons.cpp
 *
 *  Created on: 25 mar 2020
 *      Author: andr
 */

#include "JsonCommons.h"

using namespace std;

char GetFirstToken(const std::string & text, int & count)
{
	char ret = '\0';

	while (count < text.length())
	{
		if (isspace(text.at(count))) {
			count++;
		}
		else {
			ret = text.at(count);
			break;
		}
	}

//	cout << "token:" << ret <<": len:"<<count<<" tekst:"<<text<<endl;

	return ret;
}

std::string GetTextSurroundedBy(const std::string & text, int & count, char c_left, char c_right)
{
	int countLeft = 0;
	int countRight = 0;

	std::string ret;

	char c_first = GetFirstToken(text, count);
	if (count >= text.length()) return "";

	if (c_first != c_left) return "";

	count++;
	countLeft = 1;

	while (count < text.length())
	{
		char c = text.at(count);
		if (c == c_left) countLeft++;
		if (c == c_right) countRight++;

		count++;

		if (countLeft == countRight) break;
		ret += c;

	}

	return ret;
}

string GetString(const std::string & text, int & count)
{
	string ret = "";

	char c = GetFirstToken(text, count);
	if (c != '\"') return ret;
	count++;

	while (count < text.length())
	{
		c = text.at(count);
		count++;
		if (c == '\"') break;
		ret += c;
	}

	return ret;
}

int GetNumber(const std::string & text, int & count)
{
	string ret = "";

	char c = GetFirstToken(text, count);
	if (c == '-') {
		ret += c;
		count++;
	}

	while (count < text.length())
	{
		char c = text.at(count);
		if (!isdigit(c)) break;
		ret += c;
		count++;
	}

	if (ret != "")
		return stoi(ret);

	return 0;
}

float GetFloat(const std::string & text, int & count)
{
	string ret = "";

	char c = GetFirstToken(text, count);
	if (c == '-') {
		ret += c;
		count++;
	}

	while (count < text.length())
	{
		char c = text.at(count);
		if (!isdigit(c)) break;
		ret += c;
		count++;
	}

	while (count < text.length())
	{
		char c = text.at(count);
		if (!isdigit(c) && c != '.') break;
		ret += c;
		count++;
	}

	if (ret != "")
		return stof(ret);

	return 0.0f;
}

std::string ConvertToString(float value)
{
//	stringstream sstream;
//	string str;
//
//	sstream << value;
//	sstream >> str;

	return to_string(value);
//	return str;
}

int GetLastNumberFromName(string str)
{
	int i;
	for (i = str.length() - 1; i >= 0; i--)
	{
		if (!isdigit(str.at(i))) break;
	}

	if (i == str.length() - 1) return 0;

	string substr = str.substr(i+1, str.length());

	return atoi(substr.c_str());
}

JsonValueType CheckType(const std::string & text, int count)
{
	char token = GetFirstToken(text, count);
	if (token == '\0') return EJsonUnknown;

	switch(token)
	{
	case '[': return EJsonTable;
	case '{': return EJsonObject;
	case '"': return EJsonString;
	}

	if(isdigit(token) || token == '-')
	{
		int temp = GetNumber(text, count);
		if (count == text.length()) return EJsonNumber;
		char dot = text.at(count);
		if (dot == '.') return EJsonFloat;
		return EJsonNumber;
	}

	return EJsonUnknown;
}

std::string JsonVariant::value()
{
	switch (type)
	{
	case EJsonString: return "\"" + string_value + "\"";
	case EJsonNumber: return std::to_string(number_value);
	case EJsonFloat: return std::to_string(float_value);
	}

	return "";
}

void JsonVariant::parse(const std::string & text, int &count)
{
	switch (type)
	{
	case EJsonString: string_value = GetString(text, count); break;
	case EJsonNumber: number_value = GetNumber(text, count); break;
	case EJsonFloat: float_value = GetFloat(text, count); break;
	}
}

std::string JsonTable::value()
{
	std::string str;
	str = "[";
	if (ElemType == EJsonString)

	{
		for (int i = 0; i < string_values.size(); i++)
		{
			std::string substr = "\"" + string_values[i] + "\"";
			if (i > 0) substr = "," + substr;
			str += substr;
		}
	}
	if (ElemType == EJsonNumber)
	{
		for (int i = 0; i < number_values.size(); i++)
		{
			std::string substr = std::to_string(number_values[i]);
			if (i > 0) substr = "," + substr;
			str += substr;
		}
	}
	if (ElemType == EJsonFloat)
	{
		for (int i = 0; i < float_values.size(); i++)
		{
			std::string substr = std::to_string(float_values[i]);
			if (i > 0) substr = "," + substr;
			str += substr;
		}
	}
	if (ElemType == EJsonTable)
	{
		for (int i = 0; i < table_values.size(); i++)
		{
			JsonTable * table = table_values[i];

			std::string substr = table->value();

			if (i > 0) substr = "," + substr;
			str += substr;
		}
	}
	if (ElemType == EJsonObject)
	{
		for (int i = 0; i < object_values.size(); i++)
		{
			JsonObject * object = object_values[i];

			std::string substr = object->value();

			if (i > 0) substr = "," + substr;
			str += substr;
		}
	}
	str += "]";

	return str;
}

void JsonTable::parse(const std::string & text, int &count)
{
	char token = GetFirstToken(text, count);
	if (token != '[') return;

	string strval = GetTextSurroundedBy(text, count, '[', ']');
	if (strval == "") return;

//	cout << "JsonTable::parse " << strval<<endl;

	ElemType = CheckType(strval, 0);

	if (ElemType == EJsonString)
	{
		int vallen = 0;
		char token = GetFirstToken(strval, vallen);
		while (token == '"')
		{
			string str = GetString(strval, vallen);

			token = GetFirstToken(strval, vallen);
			vallen++;

			string_values.push_back(str);

			if (token != ',') break;

			token = GetFirstToken(strval, vallen);
		}
	}

	if (ElemType == EJsonNumber)
	{
		int vallen = 0;
		char token = GetFirstToken(strval, vallen);
		if (token == '\0') return;
		do
		{
			float num = GetFloat(strval, vallen);

			number_values.push_back(num);
			float_values.push_back(num);

			token = GetFirstToken(strval, vallen);
			vallen++;
		}
		while (token == ',');
	}

	if (ElemType == EJsonFloat)
	{
		int vallen = 0;
		char token = GetFirstToken(strval, vallen);
		if (token == '\0') return;
		do
		{
			float numf = GetFloat(strval, vallen);

			float_values.push_back(numf);

			token = GetFirstToken(strval, vallen);
			vallen++;
		}
		while (token == ',');
	}

	if (ElemType == EJsonTable)
	{
		int vallen = 0;
		char token = GetFirstToken(strval, vallen);
		if (token == '\0') return;
		do
		{
			JsonTable * table = new JsonTable("");

			table->parse(strval, vallen);

			table_values.push_back(table);

			token = GetFirstToken(strval, vallen);
			vallen++;
		}
		while (token == ',');
	}

	if (ElemType == EJsonObject)
	{
		int vallen = 0;
		char token = GetFirstToken(strval, vallen);
		if (token == '\0') return;
		do
		{
			JsonObject * object = new JsonObject("");

			object->parse(strval, vallen);

			object_values.push_back(object);

			token = GetFirstToken(strval, vallen);
			vallen++;
		}
		while (token == ',');
	}
}


std::string JsonObject::value()
{
	std::string str;
	str = "{";
	for (int i = 0; i < values.size(); i++)
	{
		JsonVariant * var = values[i];
		if (var->name != "")
			str += "\"" + var->name + "\" : ";

		switch (var->type)
		{
		case EJsonString:
		case EJsonNumber:
		case EJsonFloat:
			str += var->value();
			break;
		case EJsonTable:
		{
			JsonTable * table = dynamic_cast<JsonTable *>(var);
			if (table) str += table->value();
		} break;
		case EJsonObject:
		{
			JsonObject * object = dynamic_cast<JsonObject *>(var);
			if (object) str += object->value();
		} break;
		}
		if (i < values.size() - 1) str += ",";
	}
	str += "}";
	return str;
}

void JsonObject::parse(const std::string & text, int &count)
{
	string substr = GetTextSurroundedBy(text, count, '{', '}');

//	cout << "JsonObject::parse " << substr<<endl;

	int subcount = 0;
	while(subcount < substr.length())
	{
		char token;
		token = GetFirstToken(substr, subcount);

		string name;

		if (token == '\"')
		{
			name = GetString(substr, subcount);
			token = GetFirstToken(substr, subcount);
			subcount++;
			if (token != ':') continue;
		}

		JsonVariant * var;

		JsonValueType type = CheckType(substr, subcount);
		switch(type)
		{
			case EJsonString:
			case EJsonNumber:
			case EJsonFloat:
			{
				var = new JsonVariant(name, type);
//				var->type = type;
//				var->name = name;
				var->parse(substr, subcount);
			} break;
			case EJsonTable:
			{
				JsonTable * table = new JsonTable(name);
//				table->type = type;
//				table->name = name;
				table->parse(substr, subcount);
				var = table;
			} break;
			case EJsonObject:
			{
				JsonObject * object = new JsonObject(name);
//				object->type = type;
//				object->name = name;
				object->parse(substr, subcount);
				var = object;
			} break;
		}

		if (type == EJsonUnknown)
		{
			subcount++;
			continue;
		}

		values.push_back(var);
	}
}


//JsonVariant::JsonVariant()
//{
//	cout << "JsonVariant empty created" << endl;
//	cout << "type:" << to_string(type) <<endl;
//}

JsonVariant::JsonVariant(std::string name, std::string value)
{
	this->name = name;
	this->type = EJsonString;
	this->string_value = value;
}

JsonVariant::JsonVariant(std::string name, int value)
{
	this->name = name;
	this->type = EJsonNumber;
	this->number_value = value;
}

JsonVariant::JsonVariant(std::string name, float value)
{
	this->name = name;
	this->type = EJsonFloat;
	this->float_value = value;
}

JsonVariant::JsonVariant(std::string name, JsonValueType type)
{
	this->name = name;
	this->type = type;
}

JsonVariant::~JsonVariant()
{
}

//JsonTable::JsonTable()
//{
//	cout << "JsonTable created" << endl;
//}

JsonTable::JsonTable(std::string name, JsonValueType elem_type) : JsonVariant(name, EJsonTable), ElemType(elem_type)
{
//	cout << "JsonTable created" << endl;
}

JsonTable::JsonTable(std::string name) : JsonVariant(name, EJsonTable)
{
	ElemType = EJsonUnknown;
}

JsonTable::~JsonTable()
{
	for (int i = 0; i < table_values.size(); i++)
	{
		JsonTable * table = table_values[i];
		delete table;
	}
	for (int i = 0; i < object_values.size(); i++)
	{
		JsonObject * object = object_values[i];
		delete object;
	}
}

//JsonObject::JsonObject()
//{
//	cout << "JsonObject created" << endl;
//}

JsonObject::JsonObject(std::string name) : JsonVariant(name, EJsonObject)
{

}

JsonObject::~JsonObject()
{
//	cout << "JsonObject deleted" << endl;

	for (int i = 0; i < values.size(); i++)
	{
		JsonVariant * var = values[i];
		delete var;
	}
}

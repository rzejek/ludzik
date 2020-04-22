/*
 * JsonCommons.h
 *
 *  Created on: 25 mar 2020
 *      Author: andr
 */

#ifndef JSONCOMMONS_H_
#define JSONCOMMONS_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


enum JsonValueType {EJsonUnknown = 0, EJsonObject, EJsonString, EJsonNumber, EJsonFloat, EJsonTable};

struct JsonVariant
{
	JsonValueType type;
	std::string name;

	std::string string_value;
	int number_value;
	float float_value;

	virtual std::string value();
	virtual void parse(const std::string & text, int &count);

	JsonVariant(std::string name, std::string value);
	JsonVariant(std::string name, int value);
	JsonVariant(std::string name, float value);
	JsonVariant(std::string name, JsonValueType type);
	virtual ~JsonVariant();
};

class JsonObject;
struct JsonTable : JsonVariant
{
	JsonValueType ElemType;

	std::vector<std::string > string_values;
	std::vector<int > number_values;
	std::vector<float > float_values;
	std::vector<JsonTable *> table_values;
	std::vector<JsonObject *> object_values;

	virtual std::string value();
	virtual void parse(const std::string & text, int &count);

	JsonTable(std::string name, JsonValueType elem_type);
	JsonTable(std::string name);
	virtual ~JsonTable();
};

struct JsonObject : JsonVariant
{
	std::vector<JsonVariant *> values;

	virtual std::string value();
	virtual void parse(const std::string & text, int &count);

	JsonObject(std::string name);
	virtual ~JsonObject();
};

char GetFirstToken(const std::string & text, int & count);
std::string GetTextSurroundedBy(const std::string & text, int & count, char c_left, char c_right);
std::string GetString(const std::string & text, int & count);
int GetNumber(const std::string & text, int & count);
float GetFloat(const std::string & text, int & count);
std::string ConvertToString(float value);
int GetLastNumberFromName(std::string str);
JsonValueType CheckType(const std::string & text, int count);


#endif /* JSONCOMMONS_H_ */

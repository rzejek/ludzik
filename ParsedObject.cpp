/*
 * ParsedObject.cpp
 *
 *  Created on: 19 kwi 2020
 *      Author: andr
 */

#include "ParsedObject.h"


void ParsedObject::Parse(const JsonObject * obj)
{
	for (int i = 0; i < obj->values.size(); i++)
	{
		JsonVariant * var = obj->values[i];
		if (var->name == "name") name = var->string_value;
		if (var->name == "type") type = var->string_value;
		if (var->name == "id") name = var->number_value;
		if (var->name == "position")
		{
			JsonObject * obj = dynamic_cast<JsonObject *>(var);
			for (int i = 0; i < obj->values.size(); i++)
			{
				JsonVariant * var = obj->values[i];
				if (var->name == "x") position.x = var->float_value;
				if (var->name == "y") position.y = var->float_value;
				if (var->name == "z") position.z = var->float_value;
			}
		}
		if (var->name == "scale")
		{
			JsonObject * obj = dynamic_cast<JsonObject *>(var);
			for (int i = 0; i < obj->values.size(); i++)
			{
				JsonVariant * var = obj->values[i];
				if (var->name == "x") scale.x = var->float_value;
				if (var->name == "y") scale.y = var->float_value;
				if (var->name == "z") scale.z = var->float_value;
			}
		}
		if (var->name == "rotate")
		{
			JsonObject * obj = dynamic_cast<JsonObject *>(var);
			for (int i = 0; i < obj->values.size(); i++)
			{
				JsonVariant * var = obj->values[i];
				if (var->name == "x") rotate.x = var->float_value;
				if (var->name == "y") rotate.y = var->float_value;
				if (var->name == "z") rotate.z = var->float_value;
			}
		}
	}
}

JsonObject * ParsedObject::ReturnJson()
{
	JsonObject * obj = new JsonObject("");

	JsonVariant * var;

	var = new JsonVariant("name", name);
	obj->values.push_back(var);
	var = new JsonVariant("type", type);
	obj->values.push_back(var);
	var = new JsonVariant("id", id);
	obj->values.push_back(var);

	JsonObject * objpos = new JsonObject("position");
	var = new JsonVariant("x", position.x);
	objpos->values.push_back(var);
	var = new JsonVariant("y", position.y);
	objpos->values.push_back(var);
	var = new JsonVariant("z", position.z);
	objpos->values.push_back(var);
	obj->values.push_back(objpos);

	JsonObject * objscale = new JsonObject("scale");
	var = new JsonVariant("x", scale.x);
	objscale->values.push_back(var);
	var = new JsonVariant("y", scale.y);
	objscale->values.push_back(var);
	var = new JsonVariant("z", scale.z);
	objscale->values.push_back(var);
	obj->values.push_back(objscale);

	JsonObject * objrotate = new JsonObject("rotate");
	var = new JsonVariant("x", rotate.x);
	objrotate->values.push_back(var);
	var = new JsonVariant("y", rotate.y);
	objrotate->values.push_back(var);
	var = new JsonVariant("z", rotate.z);
	objrotate->values.push_back(var);
	obj->values.push_back(objrotate);

	return obj;
}

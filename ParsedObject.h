/*
 * ParsedObject.h
 *
 *  Created on: 19 kwi 2020
 *      Author: andr
 */

#ifndef PARSEDOBJECT_H_
#define PARSEDOBJECT_H_

#include "JsonCommons.h"
#include <vector>
#include <glm/glm.hpp>

struct ParsedObject
{
	std::string name;
	std::string type;
	int id;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotate;

	void Parse(const JsonObject * obj);
	JsonObject * ReturnJson();
};

#endif /* PARSEDOBJECT_H_ */

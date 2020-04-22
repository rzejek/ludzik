/*
 * ParsedObjectLoader.h
 *
 *  Created on: 19 kwi 2020
 *      Author: andr
 */

#ifndef PARSEDOBJECTLOADER_H_
#define PARSEDOBJECTLOADER_H_

#include "ParsedObject.h"
#include <vector>
#include <string>

class ParsedObjectLoader
{
public:
	std::vector<ParsedObject *> Objects;

	void Parse(std::string file);
};

#endif /* PARSEDOBJECTLOADER_H_ */

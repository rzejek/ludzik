/*
 * Game.h
 *
 *  Created on: 13 kwi 2020
 *      Author: andr
 */

#ifndef GAME_H_
#define GAME_H_

#include "Scene.h"
#include "LoadShader.h"
#include "Object.h"
#include "KeyboardModule.h"
#include "ParsedObjectLoader.h"
#include "HeroObject.h"

class Game
{
	GLuint program;
	LoadShader Shaders;

	HeroObject * Hero;
	std::vector<Object *> Objects;

	KeyboardModule Keyboard;
	ParsedObjectLoader ObjectLoader;

public:
	bool Terminated;

	void Display();
	void Tick(double time);

	Game(GLFWwindow* window);
};

#endif /* GAME_H_ */

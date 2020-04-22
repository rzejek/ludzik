/*
 * KeyboardModule.h
 *
 *  Created on: 11.04.2018
 *      Author: andr
 */

#ifndef KEYBOARDMODULE_H_
#define KEYBOARDMODULE_H_

#include "HeroObject.h"
#include <GLFW/glfw3.h>

class KeyboardModule
{
	HeroObject * Hero;
	GLFWwindow* Window;

public:
	void Tick();

	void init(HeroObject * hero, GLFWwindow* window);

	bool isSpaceBar;
	bool isEscape;
	bool isLeft;
	bool isRight;
};

#endif /* KEYBOARDMODULE_H_ */

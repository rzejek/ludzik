/*
 * KeyboardModule.cpp
 *
 *  Created on: 11.04.2018
 *      Author: andr
 */

#include "KeyboardModule.h"

void KeyboardModule::init(HeroObject * hero, GLFWwindow* window)
{
	Hero = hero;
	Window = window;

	isEscape = false;
	isSpaceBar = false;
	isLeft = false;
	isRight = false;
}

void KeyboardModule::Tick()
{
	isSpaceBar = false;
	isLeft = false;
	isRight = false;

    if (glfwGetKey(Window, GLFW_KEY_SPACE))
    {
    	if (isSpaceBar == false)
    		Hero->Jump();

    	isSpaceBar = true;
    }
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE))
    {
    	isEscape = true;
    }
    if (glfwGetKey(Window, GLFW_KEY_LEFT))
    {
    	Hero->TurnAround(0.002f);
    }
    if (glfwGetKey(Window, GLFW_KEY_RIGHT))
    {
    	Hero->TurnAround(-0.002f);
    }
    if (glfwGetKey(Window, GLFW_KEY_UP))
    {
    	Hero->Forward(0.005f);
    }
    if (glfwGetKey(Window, GLFW_KEY_DOWN))
    {
    	Hero->Forward(-0.005f);
    }
}


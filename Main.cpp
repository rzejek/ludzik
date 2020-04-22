/*
 * Main.cpp
 *
 *  Created on: 08.04.2018
 *      Author: andr
 */

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <math.h>

#include "Game.h"

using namespace std;


double time_point;
double actual_time;

int flush_fps_count;
int flush_fps_toprint_count;

int fps_count;
int fps_to_print;

int main(int argc, char ** argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 1024, "GLFW test", NULL, NULL);

    glfwMakeContextCurrent(window);

    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *renderer = glGetString(GL_RENDER);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glfwSwapInterval(0);

    glewExperimental = GL_TRUE;

    GLenum glew_status = glewInit();

    if (glew_status != GLEW_OK) {
        cerr << "glew glfw Error "<< to_string(glew_status) << "!" <<endl;
        return 1;
    }

    Game * Zgadywanka = new Game(window);

    time_point = actual_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        actual_time = glfwGetTime();
        bool redraw = true;

        if (actual_time - time_point > 1.0)
        {
            time_point = actual_time;

            fps_to_print= fps_count;
            fps_count = 0;

            flush_fps_toprint_count= flush_fps_count;
            flush_fps_count = 0;
        }

        double delta_time;
        delta_time = actual_time - time_point;

        if (delta_time*1000 > flush_fps_count)
        {
        	redraw = false;
            flush_fps_count++;

            Zgadywanka->Tick(actual_time);

        	glfwPollEvents();
        }

        if (redraw)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// | GL_STENCIL_BUFFER_BIT);

            fps_count++;

            Zgadywanka->Display();

            glFlush();

        	glfwSwapBuffers(window);
        }

        if (Zgadywanka->Terminated) break;
    }

    glfwTerminate();

	return 0;
}


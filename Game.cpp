/*
 * Game.cpp
 *
 *  Created on: 13 kwi 2020
 *      Author: andr
 */

#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Game::Game(GLFWwindow* window)
{
	Terminated = false;

    program = Shaders.CreateProgram();

    Shaders.dodajShader("vertex.glsl", GL_VERTEX_SHADER);
    Shaders.dodajShader("fragment.glsl", GL_FRAGMENT_SHADER);
    Shaders.CompileAllShaders();
    Shaders.LinkProgram();

    glUseProgram(program);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Hero = new HeroObject();
    Objects.push_back(Hero);

    Keyboard.init(Hero, window);

    ObjectLoader.Parse("maps/test.json");

    for (int i = 0; i < ObjectLoader.Objects.size(); i++)
    {
    	ParsedObject * parsed = ObjectLoader.Objects[i];
    	Object * object = new Object(parsed);

    	Objects.push_back(object);
    }
}

void Game::Display()
{
    glUseProgram(program);

    GLuint mvpLoc = glGetUniformLocation(program, "mvp");
    GLuint projectionLoc = glGetUniformLocation(program, "Projection");
    GLuint viewLoc = glGetUniformLocation(program, "View");
    GLuint modelLoc = glGetUniformLocation(program, "Model");
    GLuint lightLoc = glGetUniformLocation(program, "LightPozycja");

    glm::mat4 CameraMatrix;

    glm::vec3 CameraTarget(0.0f, 0.0f, 0.0f);
    CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f) + Hero->Position + Hero->Direction;
    glm::vec3 CameraPosition = glm::vec3(.0f, 13.0f, .0f) + Hero->Position - 16.0f*Hero->Direction;

    CameraMatrix = glm::lookAt(
            CameraPosition,
            CameraTarget,
            glm::vec3(0, 1.0f, 0)
    );

    glm::mat4 projectionMatrix = glm::perspective(
        70.0f,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        1280.0f/1024.0f, // 4.0f / 3.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        .1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        500.0f       // Far clipping plane. Keep as little as possible.
    );

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &CameraMatrix[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projectionMatrix[0][0]);

    glm::vec3 v_light_pos = CameraPosition; //(glm::vec3(.0f, 11.0f, -11.0f));

    glUniform3f(lightLoc, v_light_pos.x, v_light_pos.y, v_light_pos.z);

    for (int i = 0; i < Objects.size(); i++)
    {
    	Object * obj = Objects[i];
    	obj->Draw(modelLoc);
    }

//	DisplayScene.Draw(modelLoc);
}

void Game::Tick(double time)
{
    for (int i = 0; i < Objects.size(); i++)
    {
    	Object * obj = Objects[i];
    	obj->Tick(time);
    }
    Keyboard.Tick();
}

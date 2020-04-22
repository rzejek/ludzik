/*
 * AnimationManager.h
 *
 *  Created on: 18 kwi 2020
 *      Author: andr
 */

#ifndef ANIMATIONMANAGER_H_
#define ANIMATIONMANAGER_H_

#include "GltfBase.h"
#include "GraphObject.h"

struct Animation
{
    std::vector<float > TimePoints;
    std::vector<float > Values;
    std::vector<glm::vec3 > Values3;
    std::vector<glm::vec4 > Values4;

    std::string function;
    std::string name;

    float GetInterpolatedValue(float time);
    glm::vec4 GetInterpolatedValue4(float time);

    Animation(AnimationNode * anim_node, GltfBase * base);
    Animation();
};

class AnimationManager
{
	float AnimationsLength;
	float CurrentTime;
public:
	std::map<int, std::vector<Animation *> > Animations;
    std::map<int, Animation *> CurrentAnimations;
    std::map<int, float> StartPlayTimes;

	GltfBase * Base;

	AnimationManager();
	void Init(GltfBase * gltf_base);

	Animation * Get(int nodeid, std::string name);
	Animation * Add(int nodeid, AnimationNode * node);
	void Loop(int nodeid, Animation * anim);
	void Play(int nodeid, Animation * anim);
	void Stop(int nodeid, Animation * anim);

    float GetInterpolatedValue(int nodeid, float time);
    glm::vec4 GetInterpolatedValue4(int nodeid, float time);

    void Update(float time);
};

#endif /* ANIMATIONMANAGER_H_ */

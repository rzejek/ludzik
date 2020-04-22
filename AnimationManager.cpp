/*
 * AnimationManager.cpp
 *
 *  Created on: 18 kwi 2020
 *      Author: andr
 */

#include "AnimationManager.h"

using namespace std;

Animation::Animation(AnimationNode * anim_node, GltfBase * base)
{
	function = anim_node->Path;
	name = anim_node->Name;

	TimePoints = base->Accessors[anim_node->Input].Floats;
	Values = base->Accessors[anim_node->Output].Floats;
	Values4 = base->Accessors[anim_node->Output].getVec4();
}

Animation::Animation()
{

}

float Animation::GetInterpolatedValue(float time)
{
    if (TimePoints.size() == 0) return 0.0f;

    int i = 0;
    for ( ; i < TimePoints.size(); i++)
    {
        if (TimePoints[i] > time) break;
    }

    int j = TimePoints.size();
    for ( ; j > 0; j--)
    {
        if (TimePoints[j - 1] < time) break;
    }

    if (i == TimePoints.size() || j == TimePoints.size()) return Values[TimePoints.size()-1];

    if (i == j) return Values[i];

    float f1 = Values[i];
    float f2 = Values[j];

    float f = f1 +(f2 - f1) * (time - TimePoints[i]) / (TimePoints[j] - TimePoints[i]);

    return f;
}

glm::vec4 Animation::GetInterpolatedValue4(float time)
{
    if (TimePoints.size() == 0) return glm::vec4(0.0f);

    int i = 0;
    for ( ; i < TimePoints.size(); i++)
    {
        if (TimePoints[i] > time) break;
    }

    int j = TimePoints.size();
    for ( ; j > 0; j--)
    {
        if (TimePoints[j - 1] < time) break;
    }

    if (i == TimePoints.size() || j == TimePoints.size()) return Values4[TimePoints.size()-1];

    if (i == j) return Values4[i];

    glm::vec4 f1 = Values4[i];
    glm::vec4 f2 = Values4[j];

    glm::vec4 f = f1 +(f2 - f1) * (time - TimePoints[i]) / (TimePoints[j] - TimePoints[i]);

    return f;

}

Animation * AnimationManager::Get(int nodeid, std::string name)
{
	std::map<int, std::vector<Animation *> >::iterator iter = Animations.begin();
	while (iter != Animations.end())
	{
		if (iter->first == nodeid)
		{
			vector<Animation *> vect = iter->second;
			for (int i = 0; i < vect.size(); i++)
			{
				Animation * anim = vect[i];
				if (anim->name == name) return anim;
			}
		}
		iter++;
	}
	return NULL;
}

Animation * AnimationManager::Add(int nodeid, AnimationNode * node)
{
	Animation * animation = new Animation(node, Base);
	vector<Animation *> anims = Animations[nodeid];
	anims.push_back(animation);
	Animations[nodeid] = anims;
	return animation;
}

void AnimationManager::Loop(int nodeid, Animation * anim)
{
	CurrentAnimations[nodeid] = anim;
}

void AnimationManager::Play(int nodeid, Animation * anim)
{
	CurrentAnimations[nodeid] = anim;
	StartPlayTimes[nodeid] = CurrentTime;
//	CurrentTimes[model] = 0.0;
}

void AnimationManager::Stop(int nodeid, Animation * anim)
{
	map<int, Animation *>::iterator iter = CurrentAnimations.find(nodeid);
	CurrentAnimations.erase(iter);
	map<int, float>::iterator iter2 = StartPlayTimes.find(nodeid);
	StartPlayTimes.erase(iter2);
//	map<Model *, float>::iterator iter3 = CurrentTimes.find(model);
//	CurrentTimes.erase(iter3);
}

float AnimationManager::GetInterpolatedValue(int nodeid, float time)
{
	Update(time);
	map<int, Animation *>::iterator iter = CurrentAnimations.find(nodeid);
	map<int, float>::iterator iter2 = StartPlayTimes.find(nodeid);

	if (iter == CurrentAnimations.end() || iter2 == StartPlayTimes.end()) return 0.0f;

	Animation * animation = iter->second;
	float start = iter2->second;

	return animation->GetInterpolatedValue(time - start);
}

glm::vec4 AnimationManager::GetInterpolatedValue4(int nodeid, float time)
{
	Update(time);
	map<int, Animation *>::iterator iter = CurrentAnimations.find(nodeid);
	map<int, float>::iterator iter2 = StartPlayTimes.find(nodeid);

	if (iter == CurrentAnimations.end() || iter2 == StartPlayTimes.end()) return glm::vec4(0.0f);

	Animation * animation = iter->second;
	float start = iter2->second;

	return animation->GetInterpolatedValue4(time - start);
}

void AnimationManager::Update(float time)
{
	if (time == CurrentTime) return;

	float delta = time - CurrentTime;
	CurrentTime = time;

	map<int, Animation *>::iterator iter = CurrentAnimations.begin();
	while (iter != CurrentAnimations.end())
	{
		int nodeid = iter->first;
		Animation * animation = iter->second;

		map<int, float>::iterator iter2 = StartPlayTimes.find(nodeid);
		float start = iter2->second;

		if (time - start > AnimationsLength) StartPlayTimes[nodeid] = time;

		iter++;
	}
}

AnimationManager::AnimationManager()
{
	Base = NULL;
	AnimationsLength = 5.0;
}

void AnimationManager::Init(GltfBase * gltf_base)
{
	Base = gltf_base;
	AnimationsLength = 5.0;
}

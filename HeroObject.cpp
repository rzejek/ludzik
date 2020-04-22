/*
 * HeroObject.cpp
 *
 *  Created on: 20 kwi 2020
 *      Author: andr
 */

#include "HeroObject.h"

void HeroObject::Tick(double time)
{
	Object::Tick(time);

	if (Position.y < 0.0f) State = heroIdle;

	switch (State)
	{
		case heroIdle:
		{
			Position.y = 0.0f;
			Velocity = glm::vec3(0.0f);
			Acceleration = glm::vec3(0.0f);
		} break;
		case heroWalk:
		{
		} break;
		case heroJump:
		{
			Acceleration.y = -9.81f;
			Velocity.y += 0.001f*Acceleration.y;
			Position.y += 0.001f*Velocity.y;
		} break;
	}
}

void HeroObject::Forward(float step)
{
	Position += Direction * step;
}

void HeroObject::TurnAround(float angle)
{
	DirectionAngle += angle;
	Direction = glm::vec3(0.0f, .0f, 1.0f);

	float cosangle = glm::cos(DirectionAngle);
	float sinangle = glm::sin(DirectionAngle);
	glm::mat3 transform(cosangle, 0.0f, sinangle,
						0, 1, 0,
						-sinangle, 0, cosangle);
	Direction = Direction * transform;

	Rotation.y = DirectionAngle;
}

void HeroObject::Jump()
{
	State = heroJump;
	Velocity.y = 11.0f;
}

HeroObject::HeroObject()
{
	State = heroIdle;

	DirectionAngle = 0.0f;
	TurnAround(0.0f);

	GltfModel = new LoadGltfModel();
	GltfModel->init_vao("kwadratowy_ludzik");

	Animations.Init(&GltfModel->Base);

	Scale = glm::vec3(1.0f);
	Rotation = glm::vec3(.0f);
	Position = glm::vec3(.0f);

	map<int, Model *>::iterator iter = GltfModel->ModelsMap.begin();
	while (iter != GltfModel->ModelsMap.end())
	{
		int nodeid = iter->first;
		GltfModel->MatricesMap[nodeid] = glm::mat4(1.0f);
		iter++;
	}

	for (int i = 0; i < GltfModel->Base.Animations.size(); i++)
	{
		AnimationNode node = GltfModel->Base.Animations[i];

		int mesh_id = GltfModel->Base.Nodes[node.NodeId].MeshId;

		Animation * anim = Animations.Add(node.NodeId, &node);
		Animations.Play(node.NodeId, anim);
	}
}



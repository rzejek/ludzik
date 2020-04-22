/*
 * HeroObject.h
 *
 *  Created on: 20 kwi 2020
 *      Author: andr
 */

#ifndef HEROOBJECT_H_
#define HEROOBJECT_H_

#include "Object.h"

enum HeroState {heroIdle, heroJump, heroWalk};

class HeroObject: public Object
{
public:
	glm::vec3 Direction;
	float DirectionAngle;

	glm::vec3 Velocity;
	glm::vec3 Acceleration;

	HeroState State;

	HeroObject();
	void Tick(double time);

	void Jump();
	void TurnAround(float angle);
	void Forward(float step);
};

#endif /* HEROOBJECT_H_ */

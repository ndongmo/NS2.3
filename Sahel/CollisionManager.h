/*!
* \file CollisionManager.h
* \brief It defines all collision methods.
* \author Ndongmo Silatsa Fabrice
* \date 24-04-2017
* \version 1.0
*/

#pragma once

class Entity;

class ICollisionCallback
{
public:
	virtual bool queryCallback(Entity* e) = 0;
};

class CollisionManager
{
public:
	/*!
	* \brief Handles collision between two bodies.
	*/
	static void handleCollision(Entity* e1, Entity* e2);
};


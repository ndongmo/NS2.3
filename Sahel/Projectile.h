/*!
* \file Projectile.h
* \brief Projectile data structure (bullet, arrow, .etc).
* \author Ndongmo Silatsa Fabrice
* \date 20-06-2022
* \version 2.0
*/

#pragma once

#include "Entity.h"

class ThrowingWeapon;

class Projectile : public Entity
{
	friend ThrowingWeapon;

public:
	Projectile();
	virtual void update(Level& level, float deltaTime) override;
	virtual void handleCollision(Level& level, Entity* e, const glm::vec2& collisionDepth);

	/*!
	 * Initialize the projectile before throwing it.
	 */
	void init(Agent* agent, const Body& body, const glm::vec2& pos, const glm::vec2& dir, 
		ThrowingType type, bool bouncing, unsigned int maxBounces, float range, float speed, 
		const DamageData& damage, const EffectData& effect);

protected:
	bool m_bouncing = false;
	float m_range = 0.0f;
	float m_speed = 0.0f; 
	unsigned int m_maxBounces = 0;
	unsigned int m_currentBounces = 0;
	glm::vec2 m_direction = glm::vec2(0.0f, 0.0f);
	DamageData m_damage;
	EffectData m_effects;
	ThrowingType m_throwingType = ThrowingType::B_GUN;
	Agent* m_currentAgent = nullptr;
};

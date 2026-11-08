#include "Entity.h"
#include "Level.h"

#define MARGIN_OFFSET 50.0f

void Entity::reset()
{
	m_alive = true;
	m_active = true;
	m_collide = true;
	m_interact = false;
	m_name = "";
	m_path = "";
	m_body = Body();
	m_nodeIndex = -1;
	m_engineIndex = -1;
	m_levelIndex = -1;
}

std::ostream& operator<<(std::ostream& os, const Entity& e)
{
	return os
		<< "Entity["
		<< "name(" << e.m_name << "),"
		<< "type(" << (TypeHelper::getEntityType(e.m_type)) << "), "
		<< "alive(" << ((e.m_alive) ? "YES" : "NO") << ")]"
		<< e.m_body
		;
}

bool Entity::isType(EntityType type)
{
	return (m_type & type) != 0;
}

void Entity::applyForces(float deltaTime)
{
	m_body.applyForces(deltaTime);
}

void Entity::addChange(const MoveChange& mc)
{
	if (mc.pos.x != 0.0f || mc.pos.y != 0.0f)
		addImpulse(mc.pos);
	if (mc.scale.x != 0.0f || mc.scale.y != 0.0f)
		m_body.incrScale(mc.scale);
	if (mc.r != 0 || mc.g != 0 && mc.b != 0 || mc.alpha != 0)
		m_body.incrColor((int16_t)mc.r, (int16_t)mc.g, (int16_t)mc.b, (int16_t)mc.alpha);
}

bool Entity::checkCollision(Level& level, Entity* e)
{
	static glm::vec2 depth1, depth2;
	depth1.x = depth1.y = depth2.x = depth2.y = 0.0f;

	nodeCollision(level, e);
	e->nodeCollision(level, this);

	// now static body can also be called for collision checking
	if (MathUtils::isShapeCollide(m_body.getShape(), e->getShape(), depth1)) {
		if (e->getState() != EntityState::DYNAMIC) {
			handleCollision(level, e, depth1);
			e->handleCollision(level, this, depth2);
		}
		else {
			depth1 = depth1 * 0.5f * (e->getBody().getMass() / m_body.getMass());
			depth2 = depth1 * -0.5f * (m_body.getMass() / e->getBody().getMass());

			handleCollision(level, e, depth1);
			e->handleCollision(level, this, depth2);
		}
		return true;
	}
	return false;
}

void Entity::handleCollision(Level& level, Entity* e, const glm::vec2& collisionDepth)
{
	if (m_state == EntityState::DYNAMIC) {
		m_body.addImpulse(collisionDepth);
	}
}

void Entity::addImpulse(glm::vec2 impulse)
{
	m_body.addImpulse(impulse);
}

void Entity::addAngularImpulse(float angle)
{
	m_body.addAngularImpulse(angle);
}

void Entity::dead(Level& level)
{

}

void Entity::draw(NS2::SpriteBatch& spriteBatch) const
{
	m_body.draw(spriteBatch);
}

void Entity::drawDebug(NS2::DebugRenderer& debugRender, const NS2::ColorRGBA8& color) const
{
	m_body.drawDebug(debugRender, color);
}

void Entity::destroy(Level& level)
{

}

void Entity::updateAABB(AABB& aabb)
{
	m_body.updateAABB(aabb, m_state);
	if (m_interact) {
		float angle = m_body.getAngleRad();
		float x = roundf(MARGIN_OFFSET * cosf(angle));
		float y = roundf(MARGIN_OFFSET * sinf(angle));
		if (x == MARGIN_OFFSET)  aabb.x2 += x; else if (x == -MARGIN_OFFSET) aabb.x1 += x;
		if (y == MARGIN_OFFSET)  aabb.y2 += y; else if (y == -MARGIN_OFFSET) aabb.y1 += y;
	}
}

bool Entity::needToUpdate(AABB& aabb)
{
	return m_body.needToUpdate(aabb);
}
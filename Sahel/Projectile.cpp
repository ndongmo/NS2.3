#include "Projectile.h"
#include "Level.h"

Projectile::Projectile()
{
    m_state = EntityState::DYNAMIC;
    m_type = EntityID::ID_THROW;
    m_active = true;
    m_filter |= EntityID::ID_WFIST | EntityID::ID_WLIGHT | EntityID::ID_WSTRIKE | EntityID::ID_WTHROW;
}

void Projectile::update(Level& level, float deltaTime)
{
    
}

void Projectile::handleCollision(Level& level, Entity* e, const glm::vec2& collisionDepth)
{
    
}

void Projectile::init(Agent* agent, const Body& body, const glm::vec2& pos, const glm::vec2& dir,
    ThrowingType type, bool bouncing, unsigned int maxBounces, float range, float speed, 
    const DamageData& damage, const EffectData& effect)
{
    m_body = body;
    m_direction = dir;
    m_body.setPos(pos.x, pos.y);
    m_body.setAngle(MathUtils::calculateAngle(dir) * RADTODEG);
    m_currentAgent = agent;
    m_throwingType = type;
    m_bouncing = bouncing;
    m_maxBounces = maxBounces;
    m_range = range;
    m_speed = speed;
    m_damage = damage;
    m_effects = effect;
    m_alive = true;
    m_currentBounces = 0;
}

#include "EntityRenderer.h"
#include "Level.h"
#include "Entity.h"
#include "PhysicsEngine.h"
#include <SpriteBatch.h>
#include <DebugRenderer.h>

EntityRenderer::EntityRenderer(NS2::SpriteBatch& spriteBatch,
	NS2::DebugRenderer& debugRenderer,
	PhysicsEngine& physicsEngine)
	: Renderer(spriteBatch, debugRenderer), m_physicEngine(physicsEngine)
{
	m_debugColor.setColor(0, 0, 255, 255);
	m_debugColorEntity.setColor(255, 0, 255, 255);
	m_debugColorPhysic.setColor(0, 255, 0, 255);
	m_debugPhysic = true;
	m_debugEntity = true;
}

void EntityRenderer::render(Level& level, const glm::vec4& box)
{
	m_physicEngine.draw(m_spriteBatch, box);
}

void EntityRenderer::renderDebug(Level& level, const glm::vec4& box)
{
	m_physicEngine.drawDebug(m_debugRenderer, m_debugColor, m_debugColorPhysic, box, m_debugEntity, m_debugPhysic);
}

void EntityRenderer::renderDebug(Level& level, const glm::vec4& box, EntityID type)
{
	m_physicEngine.drawDebug(m_debugRenderer, m_debugColorEntity, m_debugColorPhysic, box, m_debugEntity, m_debugPhysic, type);
}

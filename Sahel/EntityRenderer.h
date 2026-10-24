/*!
* \file EntityRenderer.h
* \brief Entity Renderer class.
* It aims to render entities.
* \author Ndongmo Silatsa Fabrice
* \date 12-10-2017
* \version 2.0
*/

#pragma once

#include "Renderer.h"
#include "Types.h"

class Level;
class PhysicsEngine;

namespace NS2 {
	class SpriteBatch;
	class DebugRenderer;
}

class EntityRenderer : public Renderer
{
public:
	EntityRenderer(NS2::SpriteBatch& spriteBatch, 
		NS2::DebugRenderer& debugRenderer, 
		PhysicsEngine& physicsEngine);

	virtual void render( Level& level, const glm::vec4& box)override;
	virtual void renderDebug(Level& level, const glm::vec4& box)override;

	void renderDebug(Level& level, const glm::vec4& box, EntityID type);

	bool getDebugEntity() const { return m_debugEntity; }
	bool getDebugPhysic() const { return m_debugPhysic; }

	void setDebugPhysic(bool debugPhysic) { m_debugPhysic = debugPhysic; }
	void setDebugEntity(bool debugEntity) { m_debugEntity = debugEntity; }
private:
	bool m_debugEntity;
	bool m_debugPhysic;
	NS2::ColorRGBA8 m_debugColorEntity;
	NS2::ColorRGBA8 m_debugColorPhysic;
	PhysicsEngine& m_physicEngine;
};


/*!
* \file LightRenderer.h
* \brief Light Renderer class.
* It aims to render entities.
* \author Ndongmo Silatsa Fabrice
* \date 12-10-2017
* \version 2.0
*/

#pragma once

#include "Renderer.h"

class Level;
struct Light;

namespace NS2 {
	class SpriteBatch;
	class DebugRenderer;
	class GLSLProgram;
}

class LightRenderer : public Renderer
{
public:
	LightRenderer(NS2::SpriteBatch& spriteBatch, 
		NS2::DebugRenderer& debugRenderer, 
		NS2::GLSLProgram& textureProgram);
	virtual void render(Level& level, const glm::vec4& box)override;
	virtual void renderDebug(Level& level, const glm::vec4& box)override;

	void render(const Light& light);
	void renderDebug(const Light& light);

private:
	void render(const Light* light, int& activeCount);

	NS2::GLSLProgram& m_textureProgram;
};
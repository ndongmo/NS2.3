#include "LightRenderer.h"
#include "Level.h"
#include "Light.h"
#include <MathUtils.h>
#include <SpriteBatch.h>
#include <DebugRenderer.h>
#include <GLSLProgram.h>

constexpr int MAX_LIGHTS = 32;

LightRenderer::LightRenderer(NS2::SpriteBatch& spriteBatch, NS2::DebugRenderer& debugRenderer)
	: Renderer(spriteBatch, debugRenderer)
{
	m_debugColor.setColor(0, 255, 128, 255);
}

void LightRenderer::render(Level& level, const glm::vec4& box)
{
	for (auto& light : level.getLights()) {
		if (MathUtils::isBoxInView(light->getDestRect(), box)) {
			light->draw(m_spriteBatch);
		}
	}
}

void LightRenderer::renderDebug(Level& level, const glm::vec4& box)
{
	for (auto& light : level.getLights()) {
		if (MathUtils::isBoxInView(light->getDestRect(), box)) {
			m_debugRenderer.drawBox(light->getDestRect(), m_debugColor);
		}
	}
}

void LightRenderer::render(const Light& light)
{
	light.draw(m_spriteBatch);
}

void LightRenderer::renderDebug(const Light& light)
{
	m_debugRenderer.drawBox(light.getDestRect(), m_debugColor);
}

#include "LightRenderer.h"
#include "Level.h"
#include "Light.h"
#include <MathUtils.h>
#include <SpriteBatch.h>
#include <DebugRenderer.h>
#include <GLSLProgram.h>

constexpr int MAX_LIGHTS = 32;

LightRenderer::LightRenderer(NS2::SpriteBatch& spriteBatch, 
	NS2::DebugRenderer& debugRenderer, 
	NS2::GLSLProgram& textureProgram)
	: Renderer(spriteBatch, debugRenderer), 
	m_textureProgram(textureProgram)
{
	m_debugColor.setColor(0, 255, 128, 255);
}

void LightRenderer::render(Level& level, const glm::vec4& box)
{
	int activeCount = 0;

	for (size_t i = 0; i < level.getLights().size(); ++i)
	{
		if (activeCount >= MAX_LIGHTS) break;

		const auto& light = level.getLights()[i];
		if (light->active && MathUtils::isCircleInView(light->position, light->radius, box)) {
			render(light.get(), activeCount);
		}
	}
	m_textureProgram.setUniform("u_ActiveLightCount", activeCount);
}

void LightRenderer::renderDebug(Level& level, const glm::vec4& box)
{
	for (auto& light : level.getLights()) {
		if (MathUtils::isCircleInView(light->position, light->radius, box)) {
			m_debugRenderer.drawCircle(light->position, m_debugColor, light->radius);
		}
	}
}

void LightRenderer::render(const Light& light)
{
	int activeCount = 0;
	render(&light, activeCount);
	m_textureProgram.setUniform("u_ActiveLightCount", activeCount);
}

void LightRenderer::renderDebug(const Light& light)
{
	m_debugRenderer.drawCircle(light.position, m_debugColor, light.radius);
}

void LightRenderer::render(const Light* light, int& activeCount)
{
	std::string base = "u_Lights[" + std::to_string(activeCount) + "].";

	m_textureProgram.setUniform(base + "type", static_cast<int>(light->type));
	m_textureProgram.setUniform(base + "position", light->position);
	m_textureProgram.setUniform(base + "direction", light->direction);
	m_textureProgram.setUniform(base + "height", light->height);
	m_textureProgram.setUniform(base + "color", light->color);
	m_textureProgram.setUniform(base + "intensity", light->intensity);
	m_textureProgram.setUniform(base + "cutoffs", glm::vec2(light->innerAngle, light->outerAngle));
	m_textureProgram.setUniform(base + "radius", light->radius);

	activeCount++;
}

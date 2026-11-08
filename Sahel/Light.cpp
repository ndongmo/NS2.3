#include "Light.h"
#include "ObjectManager.h"
#include "MathUtils.h"

std::mt19937 Light::m_rand((unsigned int)time(nullptr));
std::uniform_int_distribution<int> Light::m_randPercent(0, 100);
const glm::vec4 Light::m_uvRect(0.0f, 0.0f, 1.0f, 1.0f);

Light::Light() = default;

void Light::draw(NS2::SpriteBatch& spriteBatch) const
{
	if (m_texture != nullptr) {
		spriteBatch.draw(m_shape.getPoints(), m_uvRect, 0, m_texture->id, m_color);
	}
}

void Light::drawDebug(NS2::DebugRenderer& debugRender, const NS2::ColorRGBA8& color) const
{
	debugRender.drawPolygon(m_shape.getPoints(), color);
}
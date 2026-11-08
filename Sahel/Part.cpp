#include "Part.h"
#include "MathUtils.h"
#include "ObjectManager.h"

std::ostream& operator<<(std::ostream& os, const Part& p)
{
	os << "collide(" << p.m_collide << "), " << p.m_shape;
	return os;
}

void Part::draw(NS2::SpriteBatch& spriteBatch) const
{
	if (!m_hidden && m_sprite.texture != nullptr && m_sprite.texture->id > 0) {
		spriteBatch.draw(m_drawShape.getPoints(), m_sprite);
	}
}

void Part::drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const
{
	if (!m_hidden) {
		m_shape.drawDebug(debugRenderer, color);
	}
}

void Part::init(PartType type, NS2::GLTexture* texture)
{
	float width = 100.0f;
	float height = 100.0f;
	if (texture != nullptr) {
		width = static_cast<float>(texture->width);
		height = static_cast<float>(texture->height);
	}
	m_type = type;
	m_scalable = false;
	m_collide = true;
	m_shape.initRotatedBox(0.0f, 0.0f, width, height, 0.0f);
	m_drawShape.initRotatedBox(0.0f, 0.0f, width, height, 0.0f);
	m_sprite.init(texture, NS2::ColorRGBA8(255, 255, 255, 255), glm::ivec2(1, 1), false, false, DepthType::DEPTH_BODY);
}

void Part::incrColor(int16_t r, int16_t g, int16_t b, int16_t a)
{
	m_sprite.color.incrColor(r, g, b, a);
}

void Part::addJoint(unsigned int joint)
{
	if (std::find(m_joints.begin(), m_joints.end(), joint) == m_joints.end()) {
		m_joints.push_back(joint);
	}
}

glm::vec2 Part::getScale() const
{
	return m_drawShape.getScale();
}

glm::vec2 Part::getPos() const
{
	return m_drawShape.getPos();
}

float Part::getAngle() const
{
	return m_drawShape.getAngle();
}

void Part::move(const Move2D& m, glm::vec2 pos)
{
	if (m.pos.x != 0 || m.pos.y != 0) incrPos(m.pos);
	if (m.a != 0) rotate(m.a, pos);
}

void Part::incrPos(glm::vec2 offset)
{
	m_drawShape.incrPos(offset);
	m_shape.incrPos(offset);

	lincrPos(offset);
}

void Part::setPos(glm::vec2 pos)
{
	incrPos(pos - m_drawShape.getPos());
}

void Part::setCenter(glm::vec2 center)
{
	incrPos(center - m_drawShape.getCenter());
}

void Part::rotate(float angle)
{
	m_drawShape.rotate(angle, m_shape.getCenter());
	m_shape.rotate(angle);

	lrotate(angle, m_shape.getCenter());
}

void Part::rotate(float angle, glm::vec2 pos)
{
	m_drawShape.rotate(angle, pos);
	m_shape.rotate(angle, pos);

	lrotate(angle, pos);
}

void Part::setAngle(float angle)
{
	m_drawShape.setAngle(angle);
	m_shape.setAngle(angle);

	lsetAngle(angle, m_shape.getCenter());
}

void Part::setAngle(float angle, glm::vec2 pos)
{
	m_drawShape.setAngle(angle, pos);
	m_shape.setAngle(angle, pos);

	lsetAngle(angle, pos);
}

void Part::scale(glm::vec2 scale)
{
	m_drawShape.scale(scale);
	m_shape.scale(scale);

	if (m_scalable) {
		m_sprite.setUvRectW(m_drawShape.getScale().x);
		m_sprite.setUvRectH(m_drawShape.getScale().y);
	}
}

void Part::scale(glm::vec2 scale, glm::vec2 pos)
{
	m_drawShape.scale(scale, pos);
	m_shape.scale(scale, pos);
}
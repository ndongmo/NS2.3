#include "BodyPart.h"
#include "MathUtils.h"
#include "ObjectManager.h"

std::ostream& operator<<(std::ostream& os, const BodyPart& p)
{
	os << "collide(" << p.m_collide << "), " << p.m_shape;
	return os;
}

void BodyPart::draw(NS2::SpriteBatch& spriteBatch) const
{
	if (!m_hidden) {
		spriteBatch.draw(m_drawShape.getConstPoints(), m_sprite);
	}
}

void BodyPart::drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const
{
	if (!m_hidden) {
		m_shape.drawDebug(debugRenderer, color);
	}
}

void BodyPart::incrColor(int16_t r, int16_t g, int16_t b, int16_t a)
{
	m_sprite.color.incrColor(r, g, b, a);
}

void BodyPart::addJoint(unsigned int joint)
{
	if (std::find(m_joints.begin(), m_joints.end(), joint) == m_joints.end()) {
		m_joints.push_back(joint);
	}
}

float BodyPart::getScaleX() const
{
	return m_drawShape.getScaleX();
}

float BodyPart::getScaleY() const
{
	return m_drawShape.getScaleY();
}

float BodyPart::getAngle() const
{
	return m_drawShape.getAngle();
}

void BodyPart::move(const Move2D& m, const glm::vec2& pos)
{
	if (m.x != 0) incrPosX(m.x);
	if (m.y != 0) incrPosY(m.y);
	if (m.a != 0) rotate(m.a, pos);
}

float BodyPart::getPosX() const
{
	return m_drawShape.getPosX();
}
float BodyPart::getPosY() const
{
	return m_drawShape.getPosY();
}

void BodyPart::incrPosX(float x)
{
	m_drawShape.incrPosX(x);
	m_shape.incrPosX(x);

	lincrPosX(x);
}

void BodyPart::incrPosY(float y)
{
	m_drawShape.incrPosY(y);
	m_shape.incrPosY(y);

	lincrPosY(y);
}

void BodyPart::incrPos(float x, float y)
{
	incrPosX(x);
	incrPosY(y);
}

void BodyPart::setPos(float x, float y)
{
	float xOffset = x - m_drawShape.getPosX();
	float yOffset = y - m_drawShape.getPosY();

	incrPos(xOffset, yOffset);
}

void BodyPart::setCenter(float x, float y)
{
	float xOffset = x - m_drawShape.getCenterX();
	float yOffset = y - m_drawShape.getCenterY();

	incrPos(xOffset, yOffset);
}

void BodyPart::rotate(float angle)
{
	m_drawShape.rotate(angle, m_shape.getConstCenter());
	m_shape.rotate(angle);

	lrotate(angle, m_shape.getConstCenter());
}

void BodyPart::rotate(float angle, const glm::vec2& pos)
{
	m_drawShape.rotate(angle, pos);
	m_shape.rotate(angle, pos);

	lrotate(angle, pos);
}

void BodyPart::setAngle(float angle)
{
	m_drawShape.setAngle(angle);
	m_shape.setAngle(angle);

	lsetAngle(angle, m_shape.getConstCenter());
}

void BodyPart::setAngle(float angle, const glm::vec2& pos)
{
	m_drawShape.setAngle(angle, pos);
	m_shape.setAngle(angle, pos);

	lsetAngle(angle, pos);
}

void BodyPart::scaleX(float x)
{
	m_drawShape.scaleX(x);
	m_shape.scaleX(x);

	if (m_scalable) {
		m_sprite.setUvRectW(m_drawShape.getScaleX());
	}
}

void BodyPart::scaleY(float y)
{
	m_drawShape.scaleY(y);
	m_shape.scaleY(y);

	if (m_scalable) {
		m_sprite.setUvRectH(m_drawShape.getScaleY());
	}
}

void BodyPart::scale(float x, float y)
{
	scaleX(x);
	scaleY(y);
}

void BodyPart::scaleX(float x, float posX)
{
	m_drawShape.scaleX(x, posX);
	m_shape.scaleX(x, posX);
}

void BodyPart::scaleY(float y, float posY)
{
	m_drawShape.scaleY(y, posY);
	m_shape.scaleY(y, posY);
}

void BodyPart::scale(float x, float y, float posX, float posY)
{
	scaleX(x, posX);
	scaleY(y, posY);
}
#include "Joint2D.h"

void Joint2D::update(const Move2D& m, const glm::vec2& pos)
{
	if (m.x != 0.0f) point.x += m.x;
	if (m.y != 0.0f) point.y += m.y;
	if (m.a != 0.0f) {
		angle += m.a;
		MathUtils::rotatePoint(point, pos, m.a);
	}
}

void Joint2D::rotate(float angle)
{
	this->angle += angle;
	this->angle = MathUtils::normalizeAngle(this->angle);
}

void Joint2D::correctValue(Move2D& m)
{
	if (value.a + m.a > upper.a) m.a = upper.a - value.a;
	if (value.a + m.a < lower.a) m.a = lower.a - value.a;
	if (value.x + m.x > upper.x) m.x = upper.x - value.x;
	if (value.x + m.x < lower.x) m.x = lower.x - value.x;
	if (value.y + m.y > upper.y) m.y = upper.y - value.y;
	if (value.y + m.y < lower.y) m.y = lower.y - value.y;

	value += m;
}

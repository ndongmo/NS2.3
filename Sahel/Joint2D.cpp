#include "Joint2D.h"

void Joint2D::init(unsigned int index, unsigned int parent, unsigned int child)
{
	this->index = index;
	this->parent = parent;
	this->child = child;
	this->angle = 0.0f;
	this->upper = Move2D();
	this->lower = Move2D();
	this->point = glm::vec2(0.0f);
}

void Joint2D::update(const Move2D& m, const glm::vec2& pos)
{
	point += m.pos;
	angle += m.a;
	if (m.a != 0.0f) {
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
	if (value.pos.x + m.pos.x > upper.pos.x) m.pos.x = upper.pos.x - value.pos.x;
	if (value.pos.x + m.pos.x < lower.pos.x) m.pos.x = lower.pos.x - value.pos.x;
	if (value.pos.y + m.pos.y > upper.pos.y) m.pos.y = upper.pos.y - value.pos.y;
	if (value.pos.y + m.pos.y < lower.pos.y) m.pos.y = lower.pos.y - value.pos.y;

	value += m;
}

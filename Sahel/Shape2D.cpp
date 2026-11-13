#include "Shape2D.h"
#include "MathUtils.h"
#include <limits>
#include <cmath>
#include <glm/gtx/norm.hpp> 

constexpr float ROT_RADIUS = 2.0f;

Shape2D::Shape2D() : m_scale(1.0f, 1.0f)
{
	initCircle(0.0f, 0.0f, 1.0f);
}

void Shape2D::init(Shape2DType type)
{
	switch (type)
	{
	case Shape2DType::CIRCLE:      initCircle(0.0f, 0.0f, 1.0f); break;
	case Shape2DType::BOX:         initBox(0.0f, 0.0f, 1.0f, 1.0f); break;
	case Shape2DType::ROTATED_BOX: initRotatedBox(0.0f, 0.0f, 1.0f, 1.0f, 0.0f); break;
	case Shape2DType::POLYGON:     initPolygon({}, 0.0f); break;
	default: break;
	}
}

void Shape2D::init(Shape2DType type, float x, float y, float width, float height)
{
	switch (type)
	{
	case Shape2DType::CIRCLE: initCircle(x, y, width * 0.5f); break;
	case Shape2DType::BOX:    initBox(x, y, width, height); break;
	case Shape2DType::ROTATED_BOX: initRotatedBox(x, y, width, height, 0.0f); break;
	case Shape2DType::POLYGON:
	{
		initPolygon({ { x, y }, { x + width, y }, { x + width, y + height }, { x, y + height } }, 0.0f);
		break;
	}
	default: break;
	}
}

void Shape2D::initCircle(float x, float y, float radius)
{
	m_points.clear();
	m_type = Shape2DType::CIRCLE;
	m_dimensions.x = m_dimensions.y = radius * 2.0f;
	m_center.x = x;
	m_center.y = y;
}

void Shape2D::initBox(float x, float y, float width, float height)
{
	m_type = Shape2DType::BOX;
	m_dimensions.x = width;
	m_dimensions.y = height;
	m_center.x = x + (width * 0.5f);
	m_center.y = y + (height * 0.5f);
	createBox(x, y, width, height);
}

void Shape2D::initRotatedBox(float x, float y, float width, float height, float angle)
{
	m_type = Shape2DType::ROTATED_BOX;
	m_angle = angle;
	m_dimensions.x = width;
	m_dimensions.y = height;
	m_center.x = x + (width * 0.5f);
	m_center.y = y + (height * 0.5f);
	createBox(x, y, width, height);
	rotate(angle);
}

void Shape2D::initPolygon(std::vector<glm::vec2> points, float angle)
{
	m_type = Shape2DType::POLYGON;
	m_angle = angle;
	m_points = std::move(points);

	float minX, minY, maxX, maxY;
	getPointsMinMax(minX, minY, maxX, maxY);
	m_dimensions.x = maxX - minX;
	m_dimensions.y = maxY - minY;

	m_center = getCentroid();
	rotate(angle);
}

void Shape2D::initTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float angle)
{
	m_type = Shape2DType::TRIANGLE;
	m_angle = angle;
	m_points.clear();
	m_points.reserve(3);
	m_points.emplace_back(x1, y1);
	m_points.emplace_back(x2, y2);
	m_points.emplace_back(x3, y3);

	float minX, minY, maxX, maxY;
	getPointsMinMax(minX, minY, maxX, maxY);
	m_dimensions.x = maxX - minX;
	m_dimensions.y = maxY - minY;

	m_center = getCentroid();
	rotate(angle);
}

void Shape2D::createBox(float x, float y, float width, float height)
{
	m_points.clear();
	m_points.resize(4);
	m_points[NS2::BL] = { x, y };
	m_points[NS2::BR] = { x + width, y };
	m_points[NS2::TR] = { x + width, y + height };
	m_points[NS2::TL] = { x, y + height };
}

void Shape2D::computeDynamicAABB(AABB& aabb) const
{
	aabb.x1 = m_center.x - m_dimensions.x;
	aabb.y1 = m_center.y - m_dimensions.y;
	aabb.x2 = m_center.x + m_dimensions.x;
	aabb.y2 = m_center.y + m_dimensions.y;
}

void Shape2D::computeStaticAABB(AABB& aabb) const
{
	float hWidth = getHalfWidth();
	float hHeight = getHalfHeight();
	aabb.x1 = m_center.x - hWidth;
	aabb.y1 = m_center.y - hHeight;
	aabb.x2 = m_center.x + hWidth;
	aabb.y2 = m_center.y + hHeight;
}

void Shape2D::updateAABB(AABB& aabb) const
{
	float minX, minY, maxX, maxY;
	getBorders(minX, minY, maxX, maxY);

	if (maxX > aabb.x2) aabb.x2 = maxX;
	if (maxY > aabb.y2) aabb.y2 = maxY;
	if (minX < aabb.x1) aabb.x1 = minX;
	if (minY < aabb.y1) aabb.y1 = minY;
}

bool Shape2D::addPoint(float x, float y)
{
	if (m_type == Shape2DType::POLYGON && m_points.size() < POLY_MAX_POINTS)
	{
		m_points.emplace_back(x, y);

		float minX, minY, maxX, maxY;
		getPointsMinMax(minX, minY, maxX, maxY);
		m_dimensions.x = maxX - minX;
		m_dimensions.y = maxY - minY;

		m_center = getCentroid();
		return true;
	}
	return false;
}

bool Shape2D::deletePoint(unsigned int pt)
{
	if (m_type == Shape2DType::POLYGON && m_points.size() > POLY_MIN_POINTS && pt < m_points.size())
	{
		// FIXED: Erased element directly to save poly winding order sequences
		m_points.erase(m_points.begin() + pt);

		float minX, minY, maxX, maxY;
		getPointsMinMax(minX, minY, maxX, maxY);
		m_dimensions.x = maxX - minX;
		m_dimensions.y = maxY - minY;

		m_center = getCentroid();
		return true;
	}
	return false;
}

void Shape2D::setType(Shape2DType newType)
{
	float minX, minY, maxX, maxY;
	getBorders(minX, minY, maxX, maxY);
	float w = maxX - minX;
	float h = maxY - minY;

	if (newType == Shape2DType::TRIANGLE)
	{
		initTriangle(minX, minY, minX + w, minY, minX + (w * 0.5f), minY + h, 0.0f);
	}
	else if (newType == Shape2DType::BOX)
	{
		initBox(minX, minY, w, h);
	}
	else if (newType == Shape2DType::ROTATED_BOX)
	{
		initRotatedBox(minX, minY, w, h, 0.0f);
	}
	else if (newType == Shape2DType::CIRCLE)
	{
		float r = std::fmaxf(w, h) * 0.5f;
		initCircle(minX + r, minY + r, r);
	}
	else if (newType == Shape2DType::POLYGON)
	{
		std::vector<glm::vec2> points(4);
		points[NS2::BL] = { minX, minY };
		points[NS2::TL] = { minX, minY + h };
		points[NS2::TR] = { minX + w, minY + h };
		points[NS2::BR] = { minX + w, minY };

		initPolygon(std::move(points), 0.0f);
	}
}

void Shape2D::setWidth(float w)
{
	if (m_type == Shape2DType::CIRCLE) {
		m_dimensions.x = m_dimensions.y = w;
	}
	else if (m_type == Shape2DType::TRIANGLE) {
		unsigned int minIndex = 0, maxIndex = 0;
		float halfW = w * 0.5f;
		m_dimensions.x = w;
		for (unsigned int i = 0; i < m_points.size(); i++) {
			if (m_points[i].x < m_points[minIndex].x) {
				minIndex = i;
			} else if (m_points[i].x > m_points[maxIndex].x) {
				maxIndex = i;
			}
		}
		m_points[minIndex].x = m_center.x - halfW;
		m_points[maxIndex].x = m_center.x + halfW;
	}
	else {
		m_dimensions.x = w;
		float halfW = w * 0.5f;
		for (auto& p : m_points) {
			p.x = m_center.x + std::copysign(halfW, p.x - m_center.x);
		}
	}
}

void Shape2D::setHeight(float h)
{
	if (m_type == Shape2DType::CIRCLE) {
		m_dimensions.x = m_dimensions.y = h;
	}
	else if (m_type == Shape2DType::TRIANGLE) {
		unsigned int minIndex = 0, maxIndex = 0;
		float halfH = h * 0.5f;
		m_dimensions.y = h;
		for (unsigned int i = 0; i < m_points.size(); i++) {
			if (m_points[i].y < m_points[minIndex].y) {
				minIndex = i;
			}
			else if (m_points[i].y > m_points[maxIndex].y) {
				maxIndex = i;
			}
		}
		m_points[minIndex].y = m_center.y - halfH;
		m_points[maxIndex].y = m_center.y + halfH;
	}
	else {
		m_dimensions.y = h;
		float halfH = h * 0.5f;
		for (auto& p : m_points) {
			p.y = m_center.y + std::copysign(halfH, p.y - m_center.y);
		}
	}
}

void Shape2D::setPoint(unsigned int index, glm::vec2 point)
{
	if (index >= m_points.size()) return;

	if (m_type == Shape2DType::CIRCLE) {
		m_center = point;
	}
	else if (m_type == Shape2DType::BOX || m_type == Shape2DType::ROTATED_BOX) {
		float angle = m_angle;
		if (m_type == Shape2DType::ROTATED_BOX && angle != 0.0f) {
			rotate(-angle);
		}
		glm::vec2 oldPoint = m_points[index];
		glm::vec2 delta = point - oldPoint;
		glm::vec2 side_sign = glm::sign(oldPoint - m_center - 0.0001f);
		m_dimensions += delta * side_sign;
		m_center += delta * 0.5f;
		for (auto& p : m_points) {
			if (oldPoint.y == p.y) {
				p.y = point.y;
			}
			if (oldPoint.x == p.x) {
				p.x = point.x;
			}
		}
		if (m_type == Shape2DType::ROTATED_BOX && angle != 0.0f) {
			rotate(angle);
		}
	}
	else {
		m_points[index] = point;
		updateDimensions();
		m_center = getCentroid();
	}
}

bool Shape2D::resize(const glm::vec2& target, const glm::vec2& init, float r)
{
	if (m_type == Shape2DType::CIRCLE)
	{
		if (MathUtils::isCircleCircleCollide(m_center, init, getHalfWidth(), r)) {
			float d = computeDist(target, init);
			m_dimensions.x += d;
			m_dimensions.y += d;
			return true;
		}
	}
	else
	{
		const size_t sz = m_points.size();
		for (size_t i = 1; i < sz; i++) {
			if (MathUtils::isLineCircleCollide(m_points[i - 1], m_points[i], init, r)) {
				move(static_cast<unsigned int>(i - 1), static_cast<unsigned int>(i), computeDist(target, init));
				return true;
			}
		}
		if (sz > 0 && MathUtils::isLineCircleCollide(m_points[sz - 1], m_points[0], init, r)) {
			move(static_cast<unsigned int>(sz - 1), 0, computeDist(target, init));
			return true;
		}
	}
	return false;
}

float Shape2D::computeDist(const glm::vec2& target, const glm::vec2& init)
{
	float dist = glm::distance(init, target);
	if (glm::distance(init, m_center) > glm::distance(target, m_center))
		dist = -dist;
	return dist;
}
void Shape2D::move(unsigned int p1, unsigned int p2, float dist)
{
	float diff_x = m_points[p2].x - m_points[p1].x;
	float diff_y = m_points[p2].y - m_points[p1].y;
	float angle = std::atan2f(diff_x, diff_y);
	float dist_x = dist * cosf(angle);
	float dist_y = -dist * sinf(angle);
	glm::vec2 dist_s(roundf(dist_x), roundf(dist_y));
	m_points[p1] += dist_s;
	m_points[p2] += dist_s;
	if (dist_s.x != 0.0f) m_dimensions.x += roundf(dist);
	if (dist_s.y != 0.0f) m_dimensions.y += roundf(dist);
	m_center = getCentroid();
}
std::ostream& operator<<(std::ostream& os, const Shape2D& s)
{
	os << (TypeHelper::getShapeType(s.m_type)) << "[";
	for (const auto& p : s.m_points) {
		os << "{" << p.x << ", " << p.y << "} ";
	}
	os << ", w=" << s.m_dimensions.x << ", h=" << s.m_dimensions.y
		<< ", angle=" << s.m_angle << "]";
	return os;
}
void Shape2D::drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const
{
	if (m_type == Shape2DType::CIRCLE)
	{
		debugRenderer.drawCircle(m_center, color, getHalfWidth());
		debugRenderer.drawCircle(m_center, color, ROT_RADIUS);
	}
	else
	{
		debugRenderer.drawPolygon(m_points, color);
		debugRenderer.drawCircle(m_center, color, ROT_RADIUS);
	}
}
void Shape2D::rotate(float angle)
{
	m_angle += angle;
	if (m_type != Shape2DType::BOX) {
		for (auto& p : m_points) {
			MathUtils::rotatePoint(p, m_center, angle);
		}
		updateDimensions();
	}
}
void Shape2D::rotate(float angle, glm::vec2 pos)
{
	if (m_type != Shape2DType::BOX) {
		for (auto& p : m_points) {
			MathUtils::rotatePoint(p, pos, angle);
		}
		MathUtils::rotatePoint(m_center, pos, angle);
		updateDimensions();
	}
	else {
		glm::vec2 pos1 = m_center;
		MathUtils::rotatePoint(pos1, pos, angle);
		glm::vec2 delta = pos1 - m_center;
		for (auto& p : m_points) {
			p += delta;
		}
		m_center += delta;
		updateDimensions();
	}
}
void Shape2D::scale(glm::vec2 scale)
{
	for (auto& p : m_points) {
		p = MathUtils::scale(p, m_center, scale);
	}
	m_dimensions *= scale;
	m_scale *= scale;
}
void Shape2D::scale(glm::vec2 scale, glm::vec2 pos)
{
	for (auto& p : m_points) {
		p = MathUtils::scale(p, pos, scale);
	}
	m_center = MathUtils::scale(m_center, pos, scale);
	m_dimensions *= scale;
}
void Shape2D::incrPos(glm::vec2 pos)
{
	for (auto& p : m_points) {
		p += pos;
	}
	m_center += pos;
}
void Shape2D::setPos(glm::vec2 pos)
{
	if (m_type == Shape2DType::CIRCLE) {
		m_center = pos;
	}
	else {
		incrPos(pos - glm::vec2(getMinX(), getMinY()));
	}
}
void Shape2D::setCenter(glm::vec2 center)
{
	incrPos(center - m_center);
}
void Shape2D::setAngle(float angle)
{
	rotate(-m_angle + angle);
}
void Shape2D::setAngle(float angle, glm::vec2 pos)
{
	rotate(-m_angle + angle, pos);
}
void Shape2D::setCircle(float centerX, float centerY, float radius)
{
	m_dimensions.x = m_dimensions.y = radius * 2.0f;
	m_center.x = centerX;
	m_center.y = centerY;
}
void Shape2D::setBox(float minX, float minY, float maxX, float maxY)
{
	m_dimensions.x = maxX - minX;
	m_dimensions.y = maxY - minY;
	m_points[NS2::BL] = { minX, minY };
	m_points[NS2::BR] = { maxX, minY };
	m_points[NS2::TR] = { maxX, maxY };
	m_points[NS2::TL] = { minX, maxY };
	m_center.x = minX + (m_dimensions.x * 0.5f);
	m_center.y = minY + (m_dimensions.y * 0.5f);
}
glm::vec2 Shape2D::getPos() const
{
	if (m_type == Shape2DType::CIRCLE)
		return m_center;
	return glm::vec2(getMinX(), getMinY());
}
void Shape2D::updateDimensions()
{
	float minX, minY, maxX, maxY;
	getBorders(minX, minY, maxX, maxY);
	m_dimensions.x = maxX - minX;
	m_dimensions.y = maxY - minY;
}
glm::vec2 Shape2D::getCentroid() const
{
	if (m_points.empty()) return glm::vec2(0.0f);
	glm::vec2 centroid(0.0f);
	for (const auto& p : m_points) {
		centroid += p;
	}
	return centroid / static_cast<float>(m_points.size());
}
float Shape2D::getMinX() const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE)
		return m_center.x - (m_dimensions.x * 0.5f);
	float minX = std::numeric_limits<float>::max();
	for (const auto& p : m_points) {
		if (p.x < minX) minX = p.x;
	}
	return minX;
}
float Shape2D::getMinY() const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE)
		return m_center.y - (m_dimensions.y * 0.5f);
	float minY = std::numeric_limits<float>::max();
	for (const auto& p : m_points) {
		if (p.y < minY) minY = p.y;
	}
	return minY;
}
float Shape2D::getMaxX() const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE)
		return m_center.x + (m_dimensions.x * 0.5f);
	float maxX = std::numeric_limits<float>::lowest();
	for (const auto& p : m_points) {
		if (p.x > maxX) maxX = p.x;
	}
	return maxX;
}
float Shape2D::getMaxY() const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE)
		return m_center.y + (m_dimensions.y * 0.5f);
	float maxY = std::numeric_limits<float>::lowest();
	for (const auto& p : m_points) {
		if (p.y > maxY) maxY = p.y;
	}
	return maxY;
}
void Shape2D::getBorders(float& minX, float& minY, float& maxX, float& maxY) const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE) {
		float hWidth = m_dimensions.x * 0.5f;
		float hHeight = m_dimensions.y * 0.5f;
		minX = m_center.x - hWidth;
		minY = m_center.y - hHeight;
		maxX = m_center.x + hWidth;
		maxY = m_center.y + hHeight;
	}
	else {
		getPointsMinMax(minX, minY, maxX, maxY);
	}
}
float Shape2D::getFartherDis(const glm::vec2& center) const
{
	if (m_type == Shape2DType::CIRCLE) {
		return glm::distance(center, m_center) + getHalfWidth();
	}
	float maxDisSq = 0.0f;
	for (const auto& p : m_points) {
		float disSq = glm::distance2(center, p);
		if (disSq > maxDisSq) maxDisSq = disSq;
	}
	return std::sqrt(maxDisSq);
}
// Private single-pass helper optimized to run bound evaluations efficiently
void Shape2D::getPointsMinMax(float& minX, float& minY, float& maxX, float& maxY) const
{
	if (m_points.empty()) {
		minX = minY = maxX = maxY = 0.0f;
		return;
	}
	minX = maxX = m_points[0].x;
	minY = maxY = m_points[0].y;
	const size_t sz = m_points.size();
	for (size_t i = 1; i < sz; ++i) {
		const float x = m_points[i].x;
		const float y = m_points[i].y;
		if (x < minX) minX = x;
		if (x > maxX) maxX = x;
		if (y < minY) minY = y;
		if (y > maxY) maxY = y;
	}
}
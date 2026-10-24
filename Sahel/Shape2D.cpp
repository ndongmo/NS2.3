#include "Shape2D.h"
#include "MathUtils.h"

#include <limits>

#define ROT_RADIUS 2.0f
#define POLY_MAX_POINTS 8
#define POLY_MIN_POINTS 4

Shape2D::Shape2D() : m_scale(1.0f, 1.0f)
{
	initCircle(0, 0, 1.0f);
}
void Shape2D::init(Shape2DType type)
{
	if (type == Shape2DType::CIRCLE)
		initCircle(0, 0, 1.0f);
	else if (type == Shape2DType::BOX)
		initBox(0.0f, 0.0f, 1.0f, 1.0f);
	else if (type == Shape2DType::ROTATED_BOX)
		initRotatedBox(0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	else if (type == Shape2DType::POLYGON)
		initPolygon(std::vector<glm::vec2>(), 0.0f);
}

void Shape2D::init(Shape2DType type, float x, float y, float width, float height)
{
	if (type == Shape2DType::CIRCLE)
		initCircle(x, y, width / 2.0f);
	else if (type == Shape2DType::BOX)
		initBox(x, y, width, height);
	else if (type == Shape2DType::ROTATED_BOX)
		initRotatedBox(x, y, width, height, 0.0f);
	else if (type == Shape2DType::POLYGON) {
		initPolygon(std::vector<glm::vec2> {
			{ x, y }, { x + width, y }, { x + width, y + height }, { x, y + height }}, 0.0f);
	}
}

void Shape2D::initCircle(float x, float y, float radius)
{
	m_type = Shape2DType::CIRCLE;
	m_dimensions.x = m_dimensions.y = radius * 2;
	m_center.x = x;
	m_center.y = y;
	createBox(x - radius, y - radius, radius * 2, radius * 2);
}

void Shape2D::initBox(float x, float y, float width, float height)
{
	m_type = Shape2DType::BOX;
	m_dimensions.x = width;
	m_dimensions.y = height;
	m_center.x = x + (width / 2.0f);
	m_center.y = y + (height / 2.0f);
	createBox(x, y, width, height);
}

void Shape2D::initRotatedBox(float x, float y, float width, float height, float angle)
{
	m_type = Shape2DType::ROTATED_BOX;
	m_angle = angle;
	m_dimensions.x = width;
	m_dimensions.y = height;
	m_center.x = x + (width / 2.0f);
	m_center.y = y + (height / 2.0f);
	createBox(x, y, width, height);
	rotate(angle);
}

void Shape2D::initPolygon(const std::vector<glm::vec2>& points, float angle)
{
	m_type = Shape2DType::POLYGON;
	m_angle = angle;
	m_points = points;
	m_dimensions.x = getMaxX() - getMinX();
	m_dimensions.y = getMaxY() - getMinY();
	m_center = getCentroid();
	rotate(angle);
}

void Shape2D::initTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float angle)
{
	m_type = Shape2DType::TRIANGLE;
	m_angle = angle;
	m_points.clear();
	m_points.emplace_back(x1, y1);
	m_points.emplace_back(x2, y2);
	m_points.emplace_back(x3, y3);
	m_dimensions.x = getMaxX() - getMinX();
	m_dimensions.y = getMaxY() - getMinY();
	m_center = getCentroid();
	rotate(angle);
}

void Shape2D::createBox(float x, float y, float width, float height)
{
	m_points.resize(4);

	m_points[NS2::BL].x = x;
	m_points[NS2::BL].y = y;
	m_points[NS2::BR].x = x + width;
	m_points[NS2::BR].y = y;
	m_points[NS2::TR].x = x + width;
	m_points[NS2::TR].y = y + height;
	m_points[NS2::TL].x = x;
	m_points[NS2::TL].y = y + height;
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
	aabb.x1 = m_center.x - getHalfWidth();
	aabb.y1 = m_center.y - getHalfHeight();
	aabb.x2 = m_center.x + getHalfWidth();
	aabb.y2 = m_center.y + getHalfHeight();
}

void Shape2D::updateAABB(AABB& aabb) const
{
	float value = getMaxX();
	if (value > aabb.x2) aabb.x2 = value;
	value = getMaxY();
	if (value > aabb.y2) aabb.y2 = value;
	value = getMinX();
	if (value < aabb.x1) aabb.x1 = value;
	value = getMinY();
	if (value < aabb.y1) aabb.y1 = value;
}

void Shape2D::updateDestRect(glm::vec4& destRect) const
{
	destRect.x = getPosX();
	destRect.y = getPosY();
	destRect.z = getWidth();
	destRect.w = getWidth();
}

bool Shape2D::addPoint(float x, float y)
{
	if (m_type == Shape2DType::POLYGON && m_points.size() < POLY_MAX_POINTS)
	{
		m_points.push_back(glm::vec2(x, y));

		m_dimensions.x = getMaxX() - getMinX();
		m_dimensions.y = getMaxY() - getMinY();
		m_center = getCentroid();
		return true;
	}
	return false;
}
bool Shape2D::deletePoint(unsigned int pt)
{
	if (m_type == Shape2DType::POLYGON && m_points.size() > POLY_MIN_POINTS)
	{
		m_points[pt] = m_points.back();
		m_points.pop_back();

		m_dimensions.x = getMaxX() - getMinX();
		m_dimensions.y = getMaxY() - getMinY();
		m_center = getCentroid();
		return true;
	}
	return false;
}
void Shape2D::setType(Shape2DType newType)
{
	float x = getMinX();
	float y = getMinY();
	float w = getMaxX() - x;
	float h = getMaxY() - y;

	if (newType == Shape2DType::TRIANGLE)
	{
		initTriangle(x, y, x + w, y, x + (w / 2.0f), y + h, 0.0f);
	}
	else if (newType == Shape2DType::BOX)
	{
		initBox(x, y, w, h);
	}
	else if (newType == Shape2DType::ROTATED_BOX)
	{
		initRotatedBox(x, y, w, h, 0.0f);
	}
	else if (newType == Shape2DType::CIRCLE)
	{
		float r = std::fmaxf(w, h) / 2.0f;
		initCircle(x + r, y + r, r);
	}
	else if (newType == Shape2DType::POLYGON)
	{
		std::vector<glm::vec2> points(4);
		points[NS2::BL].x = x; points[NS2::BL].y = y;
		points[NS2::TL].x = x; points[NS2::TL].y = y + h;
		points[NS2::TR].x = x + w; points[NS2::TR].y = y + h;
		points[NS2::BR].x = x + w; points[NS2::BR].y = y;

		initPolygon(points, 0.0f);
	}
}

void Shape2D::setHeight(float h)
{
	if (m_type == Shape2DType::CIRCLE) {
		m_dimensions.x = m_dimensions.y = h;
	}
	else {
		m_dimensions.y = h;
		getBL().y = m_center.y - (h / 2.0f);
		getBR().y = m_center.y - (h / 2.0f);
		getTL().y = m_center.y + (h / 2.0f);
		getTR().y = m_center.y + (h / 2.0f);
	}
}
void Shape2D::setPosX(float x)
{
	if (m_type == Shape2DType::CIRCLE) {
		m_center.x = x;
	}
	else {
		float minX = x - getMinX();
		incrPosX(minX);
	}
}
void Shape2D::setPosY(float y)
{
	if (m_type == Shape2DType::CIRCLE) {
		m_center.y = y;
	}
	else {
		float minY = y - getMinY();
		incrPosY(minY);
	}
}
void Shape2D::setWidth(float w)
{
	if (m_type == Shape2DType::CIRCLE) {
		m_dimensions.x = m_dimensions.y = w;
	}
	else {
		m_dimensions.x = w;
		getBL().x = m_center.x - (w / 2.0f);
		getBR().x = m_center.x + (w / 2.0f);
		getTL().x = m_center.x - (w / 2.0f);
		getTR().x = m_center.x + (w / 2.0f);
	}
}
void Shape2D::setPointX(unsigned int index, const glm::vec2& pt)
{
	if (m_type == Shape2DType::CIRCLE)
	{
		m_dimensions.x = m_dimensions.y = pt.x;
	}
	else if (m_type == Shape2DType::BOX)
	{
		if (pt.x < m_center.x) {
			getBL().x = pt.x;
			getTL().x = pt.x;
			m_dimensions.x = getBR().x - pt.x;
			m_center.x = pt.x + (m_dimensions.x / 2.0f);
		}
		else {
			getBR().x = pt.x;
			getTR().x = pt.x;
			m_dimensions.x = pt.x - getBL().x;
			m_center.x = pt.x - (m_dimensions.x / 2.0f);
		}
	}
	else if (m_type == Shape2DType::ROTATED_BOX)
	{
		float angle = m_angle;
		if (angle != 0.0f) rotate(-angle);

		if (pt.x < m_center.x) {
			getBL().x = pt.x;
			getTL().x = pt.x;
			m_dimensions.x = getBR().x - pt.x;
			m_center.x = pt.x + (m_dimensions.x / 2.0f);
		}
		else {
			getBR().x = pt.x;
			getTR().x = pt.x;
			m_dimensions.x = pt.x - getBL().x;
			m_center.x = pt.x - (m_dimensions.x / 2.0f);
		}

		if (angle != 0.0f) rotate(angle);
	}
	else
	{
		m_points[index].x = pt.x;
		m_dimensions.x = getMaxX() - getMinX();
		m_center = getCentroid();
	}
}

void Shape2D::setPointY(unsigned int index, const glm::vec2& pt)
{
	if (m_type == Shape2DType::CIRCLE)
	{
		return;
	}
	else if (m_type == Shape2DType::BOX)
	{
		if (pt.y < m_center.y) {
			getBL().y = pt.y;
			getBR().y = pt.y;
			m_dimensions.y = getTL().y - pt.y;
			m_center.y = pt.y + (m_dimensions.y / 2.0f);
		}
		else {
			getTL().y = pt.y;
			getTR().y = pt.y;
			m_dimensions.y = pt.y - getBL().y;
			m_center.y = pt.y - (m_dimensions.y / 2.0f);
		}
	}
	else if (m_type == Shape2DType::ROTATED_BOX)
	{
		float angle = m_angle;
		if (angle != 0.0f) rotate(-angle);

		if (pt.y < m_center.y) {
			getBL().y = pt.y;
			getBR().y = pt.y;
			m_dimensions.y = getTL().y - pt.y;
			m_center.y = pt.y + (m_dimensions.y / 2.0f);
		}
		else {
			getTL().y = pt.y;
			getTR().y = pt.y;
			m_dimensions.y = pt.y - getBL().y;
			m_center.y = pt.y - (m_dimensions.y / 2.0f);
		}

		if (angle != 0.0f) rotate(angle);
	}
	else
	{
		m_points[index].y = pt.y;
		m_dimensions.y = getMaxY() - getMinY();
		m_center = getCentroid();
	}
}

bool Shape2D::resize(const glm::vec2& target, const glm::vec2& init, float r)
{
	if (m_type == Shape2DType::CIRCLE)
	{
		if (MathUtils::isCircleCircleCollide(m_center, init, getHalfWidth(), r)) {
			m_dimensions.y = m_dimensions.x += computeDist(target, init);
			return true;
		}
	}
	else
	{
		for (uint8_t i = 1; i < m_points.size(); i++) {
			if (MathUtils::isLineCircleCollide(m_points[i - 1], m_points[i], init, r)) {
				move(i - 1, i, computeDist(target, init));
				return true;
			}
		}
		if (MathUtils::isLineCircleCollide(m_points[m_points.size() - 1], m_points[0], init, r)) {
			move(static_cast<int>(m_points.size()) - 1, 0, computeDist(target, init));
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
	float dist_x = dist * cos(angle);
	float dist_y = -dist * sin(angle);

	glm::vec2 dist_s(roundf(dist_x), roundf(dist_y));
	m_points[p1] += dist_s;
	m_points[p2] += dist_s;
	if (dist_s.x != 0) m_dimensions.x += roundf(dist);
	if (dist_s.y != 0) m_dimensions.y += roundf(dist);
	m_center = getCentroid();
}

std::ostream& operator<<(std::ostream& os, const Shape2D& s)
{
	os << (TypeHelper::getShapeType(s.m_type)) << "[";
	for (auto& p : s.m_points){
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
		for (uint8_t i = 0; i < m_points.size(); i++){
			MathUtils::rotatePoint(m_points[i], m_center, angle);
		}
		updateDimensions();
	}
}

void Shape2D::rotate(float angle, const glm::vec2& pos)
{
	if (m_type != Shape2DType::BOX) {
		for (uint8_t i = 0; i < m_points.size(); i++){
			MathUtils::rotatePoint(m_points[i], pos, angle);
		}
		MathUtils::rotatePoint(m_center, pos, angle);
		updateDimensions();
	}
	else {
		glm::vec2 pos1 = m_center;
		MathUtils::rotatePoint(pos1, pos, angle);
		pos1 -= m_center;
		for (uint8_t i = 0; i < m_points.size(); i++) {
			m_points[i] += pos1;
		}
		m_center += pos1;
		updateDimensions();
	}
}

void Shape2D::scaleX(float x)
{
	if (x > 0.0f) {
		for (uint8_t i = 0; i < m_points.size(); i++) {
			m_points[i].x = MathUtils::scale(m_points[i].x, m_center.x, x);
		}
		m_dimensions.x *= x;
		m_scale.x *= x;
	}
}
void Shape2D::scaleY(float y)
{
	if (y > 0.0f) {
		for (uint8_t i = 0; i < m_points.size(); i++) {
			m_points[i].y = MathUtils::scale(m_points[i].y, m_center.y, y);
		}
		m_dimensions.y *= y;
		m_scale.y *= y;
	}
}
void Shape2D::scale(float x, float y)
{
	scaleX(x);
	scaleY(y);
}
void Shape2D::scaleX(float x, float posX)
{
	if (x > 0.0f) {
		for (uint8_t i = 0; i < m_points.size(); i++) {
			m_points[i].x = MathUtils::scale(m_points[i].x, posX, x);
		}
		m_center.x = MathUtils::scale(m_center.x, posX, x);
		m_dimensions.x *= x;
	}
}
void Shape2D::scaleY(float y, float posY)
{
	if (y > 0.0f) {
		for (uint8_t i = 0; i < m_points.size(); i++) {
			m_points[i].y = MathUtils::scale(m_points[i].y, posY, y);
		}
		m_center.y = MathUtils::scale(m_center.y, posY, y);
		m_dimensions.y *= y;
	}
}
void Shape2D::scale(float x, float y, float posX, float posY)
{
	scaleX(x, posX);
	scaleY(y, posY);
}

void Shape2D::incrPosX(float x)
{
	for (uint8_t i = 0; i < m_points.size(); i++){
		m_points[i].x += x;
	}
	m_center.x += x;
}
void Shape2D::incrPosY(float y)
{
	for (uint8_t i = 0; i < m_points.size(); i++){
		m_points[i].y += y;
	}
	m_center.y += y;
}

void Shape2D::incrPos(float x, float y)
{
	incrPosX(x);
	incrPosY(y);
}

void Shape2D::setPos(float x, float y)
{
	if (m_type == Shape2DType::CIRCLE) {
		m_center.x = x;
		m_center.y = y;
	} else {
		float minX = x - getMinX();
		float minY = y - getMinY();
		incrPos(minX, minY);
	}
}

void Shape2D::setCenter(float x, float y)
{
	float minX = x - m_center.x;
	float minY = y - m_center.y;

	incrPos(minX, minY);
}

void Shape2D::setAngle(float angle)
{
	rotate(-m_angle + angle);
}

void Shape2D::setAngle(float angle, const glm::vec2& pos)
{
	rotate(-m_angle + angle, pos);
}

void Shape2D::setCircle(float centerX, float centerY, float radius)
{
	m_dimensions.x = m_dimensions.y = radius * 2;

	m_center.x = centerX;
	m_center.y = centerY;
}

void Shape2D::setBox(float minX, float minY, float maxX, float maxY)
{
	m_dimensions.x = maxX - minX;
	m_dimensions.y = maxY - minY;

	m_points[NS2::BL].x = minX;
	m_points[NS2::BL].y = minY;
	m_points[NS2::BR].x = maxX;
	m_points[NS2::BR].y = minY;
	m_points[NS2::TR].x = maxX;
	m_points[NS2::TR].y = maxY;
	m_points[NS2::TL].x = minX;
	m_points[NS2::TL].y = maxY;

	m_center.x = m_points[NS2::BL].x + (m_dimensions.x / 2.0f);
	m_center.y = m_points[NS2::BL].y + (m_dimensions.y / 2.0f);
}

glm::vec2 Shape2D::getPos() const
{ 
	if (m_type == Shape2DType::CIRCLE)
		return m_center;
	else
		return glm::vec2(getMinX(), getMinY());
}

float Shape2D::getPosX() const
{
	if (m_type == Shape2DType::CIRCLE)
		return m_center.x;
	else
		return getMinX();
}
float Shape2D::getPosY() const
{
	if (m_type == Shape2DType::CIRCLE)
		return m_center.y;
	else
		return getMinY();
}

void Shape2D::updateDimensions()
{
	float minX, minY, maxX, maxY;
	getBorders(minX, minY, maxX, maxY);
	m_dimensions.x = maxX - minX;
	m_dimensions.y = maxY - minY;
}

glm::vec2  Shape2D::getCentroid() const
{
	glm::vec2 centroid(0.0f);
	for (auto& p : m_points){
		centroid += p;
	}
	centroid /= m_points.size();
	return centroid;
}

float Shape2D::getMinX() const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE)
		return m_center.x - (m_dimensions.x / 2.0f);
	else
	{
		float minX = std::numeric_limits<float>::max();
		for (auto& p : m_points){
			if (p.x < minX)
				minX = p.x;
		}
		return minX;
	}
}

float Shape2D::getMinY() const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE)
		return m_center.y - (m_dimensions.y / 2.0f);
	else
	{
		float minY = std::numeric_limits<float>::max();
		for (auto& p : m_points){
			if (p.y < minY)
				minY = p.y;
		}
		return minY;
	}
}

float Shape2D::getMaxX() const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE)
		return m_center.x + (m_dimensions.x / 2.0f);
	else
	{
		float maxX = std::numeric_limits<float>::min();
		for (auto& p : m_points){
			if (p.x > maxX)
				maxX = p.x;
		}
		return maxX;
	}
}

float Shape2D::getMaxY() const
{
	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE)
		return m_center.y + (m_dimensions.y / 2.0f);
	else
	{
		float maxY = std::numeric_limits<float>::min();
		for (auto& p : m_points){
			if (p.y > maxY)
				maxY = p.y;
		}
		return maxY;
	}
}

void Shape2D::getBorders(float& minX, float& minY, float& maxX, float& maxY) const
{
	minX = std::numeric_limits<float>::max();
	minY = std::numeric_limits<float>::max();
	maxX = std::numeric_limits<float>::min();
	maxY = std::numeric_limits<float>::min();

	if (m_type == Shape2DType::BOX || m_type == Shape2DType::CIRCLE) {
		minX = m_center.x - (m_dimensions.x / 2.0f);
		minY = m_center.y - (m_dimensions.y / 2.0f);
		maxX = m_center.x + (m_dimensions.x / 2.0f);
		maxY = m_center.y + (m_dimensions.y / 2.0f);
	}
	else {
		for (auto& p : m_points) {
			if (p.x < minX) minX = p.x;
			if (p.y < minY) minY = p.y;
			if (p.x > maxX) maxX = p.x;
			if (p.y > maxY) maxY = p.y;
		}
	}
}

float Shape2D::getFartherDis(const glm::vec2& center) const
{
	float dis = 0.0f;
	float tmp = 0.0f;

	if (m_type == Shape2DType::CIRCLE) {
		dis = glm::distance(center, m_center) + getHalfWidth();
	}
	else {
		for (auto& p : m_points) {
			tmp = glm::distance(center, p);
			if (tmp > dis)
				dis = tmp;
		}
	}
	return dis;
}


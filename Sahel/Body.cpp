#include "Body.h"
#include "MathUtils.h"

#pragma region PUBLIC CLASS METHODS ---------------------------------------------------------------------------------

void Body::init(float posX, float posY, float scaleX, float scaleY, float angle)
{
	m_initAngle = angle;
	m_initPos.x = posX;
	m_initPos.y = posY;
	m_initScale.x = scaleX;
	m_initScale.y = scaleY;

	rotate(m_initAngle);
	scale(m_initScale);
	setPos(m_initPos);
}

void Body::reinit()
{
	m_needUpdate = true;

	// order is important!
	setAngle(m_initAngle);
	setScale(m_initScale);
	setPos(m_initPos);
}

std::ostream& operator<<(std::ostream& os, const Body& b)
{
	os << "Body[mass(" << b.m_mass << "),";
	for (uint16_t i = 0; i < b.m_parts.size(); i++)
		os << "part" << i << ": [" << b.m_parts[i] << "];";
	os << "]";
	return os;
}

void Body::attach(ITransform* body, unsigned int partIndex)
{
	if (body != nullptr && partIndex < m_parts.size()) {
		m_parts[partIndex].lattach(body);
	}
}
void Body::detach(ITransform* body, unsigned int partIndex)
{
	if (body != nullptr && partIndex < m_parts.size()) {
		m_parts[partIndex].ldetach(body);
	}
}
void Body::detachAll()
{
	for (auto& part : m_parts)
		part.ldetachAll();
}
void Body::setPart(unsigned int index, Part& part)
{
	part.m_index = index;
	m_parts[index] = part;
}

void Body::addAngularImpulse(float angle)
{
	m_angularVelocity += angle;
}
void Body::addImpulse(glm::vec2 impulse)
{
	m_forces += impulse;
}
void Body::applyImpulse(float friction, float deltaTime)
{
	m_forces *= friction;

	incrPos(m_forces);
	m_forces = glm::vec2(0.0f);
}

void Body::updateAABB(AABB& aabb, EntityState state)
{
	if (state == EntityState::DYNAMIC) {
		m_shape.computeDynamicAABB(aabb);
	}
	else if (state == EntityState::KINEMATIC) {
		m_shape.computeStaticAABB(aabb);
	}
	else {
		if (m_parts.size() == 1) {
			m_parts[0].getShape().computeStaticAABB(aabb);
		}
		else {
			m_shape.computeStaticAABB(aabb);
		}
	}
	m_needUpdate = false;
}

bool Body::needToUpdate(AABB& aabb) const
{
	return m_needUpdate && MathUtils::isBoxOutView(m_shape, aabb);
}

void Body::applyForces(float deltaTime)
{
	if (m_angularVelocity != 0.0f || m_forces.x != 0.0f || m_forces.y != 0.0f) {
		rotate(m_angularVelocity * deltaTime * m_angularSpeed);
		incrPos(m_forces);

		m_forces = glm::vec2(0.0f);
		m_angularVelocity = 0.0f;
		m_needUpdate = true;
	}
}
int Body::getParentPart(unsigned int partIndex) const
{
	for (auto& j : m_joints) {
		if (j.son == partIndex)
			return j.parent;
	}
	return -1;
}
int Body::getParentJoint(unsigned int partIndex) const
{
	for (auto& j : m_joints) {
		if (j.son == partIndex)
			return j.index;
	}
	return -1;
}
float Body::getDepth() const
{
	float depth = DepthType::DEPTH_GROUND;

	for (auto& p : m_parts) {
		if (p.getSprite().depth > depth)
			depth = p.getSprite().depth;
	}
	return depth;
}
glm::vec2 Body::getCenter() const
{
	if (m_parts.empty())
		return m_shape.getCenter();
	else if (m_parts[0].m_parentJoint >= 0) 
		return  m_joints[m_parts[0].m_parentJoint].point;
	else 
		return m_parts[0].getShape().getCenter();
}
glm::vec2 Body::getPos() const
{
	return getShape().getPos();
}
glm::vec2 Body::getMidleFacePoint() const
{
	return (getRealShape().getPos(NS2::BR) + getRealShape().getPos(NS2::TR)) / 2.0f;
}
#pragma endregion 

#pragma region PROTECTED CLASS METHODS ---------------------------------------------------------------------------------

void Body::updateShape()
{
	if (m_shape.getType() == Shape2DType::CIRCLE) {
		updateCircle();
	}
	else if (m_shape.getType() == Shape2DType::POLYGON) {
		updatePolygon();
	}
	else {
		updateBox();
	}
}
void Body::updateShape(float angle)
{
	if (angle == 0.0f) return;

	if (m_shape.getType() == Shape2DType::BOX)
		updateBox();

	m_shape.rotate(angle * DEGTORAD);
}
void Body::updatePolygon()
{
	std::vector<glm::vec2> points;
	for (auto& p : m_parts){
		if (p.canCollide()) {
			for (uint16_t i = 0; i < p.getShape().getPoints().size(); i++)
				points.push_back(p.getShape().getPoints().at(i));
		}
	}

	m_shape.initPolygon(std::move(MathUtils::convexHull(points)), 0.0f);
}
void Body::updateCircle()
{
	glm::vec2 center(0.0f, 0.0f);
	float farther = 0.0f, tmp;
	int i = 0;

	for (auto& p : m_parts){
		if (p.canCollide()) {
			i++;
			center += p.getShape().getCenter();
		}
	}
	center /= i;

	for (auto& p : m_parts){
		if (p.canCollide()) {
			tmp = p.getShape().getFartherDis(center);
			if (farther < tmp)
				farther = tmp;
		}
	}

	m_shape.setCircle(center.x, center.y, farther);
}

void Body::updateBox()
{
	float minX = m_parts[0].getShape().getMinX();
	float minY = m_parts[0].getShape().getMinY();
	float maxX = m_parts[0].getShape().getMaxX();
	float maxY = m_parts[0].getShape().getMaxY();
	float x = 0.0f, y = 0.0f;

	for (uint16_t i = 1; i < m_parts.size(); i++) {
		if (m_parts[i].canCollide()) {
			x = m_parts[i].getShape().getMinX();
			y = m_parts[i].getShape().getMinY();
			if (x < minX) minX = x;
			if (y < minY) minY = y;
			x = m_parts[i].getShape().getMaxX();
			y = m_parts[i].getShape().getMaxY();
			if (x > maxX) maxX = x;
			if (y > maxY) maxY = y;
		}
	}
	m_shape.setBox(minX, minY, maxX, maxY);
}

void Body::move(unsigned int partIndex, glm::vec2 value)
{
	float orientation = getAngleRad() + m_orientation;
	float cosAngle = cos(orientation);
	float sinAngle = sin(orientation);

	float x = value.x * cosAngle - value.y * sinAngle;
	float y = value.x * sinAngle + value.y * cosAngle;

	incrPos(partIndex, glm::vec2(x, y));
}
void Body::move(unsigned int partIndex, const Move2D& m, glm::vec2 rotPoint)
{
	m_parts[partIndex].move(m, rotPoint);

	for (auto& j : m_parts[partIndex].m_joints) {
		move(m_joints[j].son, m, rotPoint);
		m_joints[j].update(m, rotPoint);
	}
}
void Body::move(const Move2D& m)
{
	if (m.a != 0.0f) {
		rotate(0, m.a);
	}
	incrPos(m.pos);
}
void Body::move(unsigned int jointIndex, float x, float y, float angle)
{
	Move2D m(x, y, angle * DEGTORAD);
	m_joints[jointIndex].correctValue(m);

	float orientation = m_orientation + m_joints[jointIndex].angle + getAngleRad();
	float cosAngle = cos(orientation);
	float sinAngle = sin(orientation);
	float mx = m.pos.x;

	m.a = m.a;
	m.pos.x = mx * cosAngle - m.pos.y * sinAngle;
	m.pos.y = mx * sinAngle + m.pos.y * cosAngle;

	m_joints[jointIndex].point += m.pos;

	move(m_joints[jointIndex].son, m, m_joints[jointIndex].point);
}
void Body::move(unsigned int jointIndex, const MoveChange& mc)
{
	// scaling and rotating do not work yet!!!
	if ((mc.scale.x != 0 || mc.scale.y != 0) && mc.angle == 0) {
		incrScale(m_joints[jointIndex].son, mc.scale);
	}
	else if (mc.pos.x != 0 || mc.pos.y != 0 || mc.angle != 0) {
		move(jointIndex, mc.pos.x, mc.pos.y, mc.angle);
	}
	if (mc.r != 0 || mc.g != 0 || mc.b != 0 || mc.alpha != 0) {
		m_parts[m_joints[jointIndex].son].incrColor((int16_t)mc.r, 
			(int16_t)mc.g, (int16_t)mc.b, (int16_t)mc.alpha);
	}
}

void Body::incrColor(unsigned int partIndex, int16_t r, int16_t g, int16_t b, int16_t a)
{
	m_parts[partIndex].incrColor(r, g, b, a);
}

void Body::incrColor(int16_t r, int16_t g, int16_t b, int16_t a)
{
	for (auto& p : m_parts) {
		p.incrColor(r, g, b, a);
	}
}

void Body::incrScale(unsigned int partIndex, glm::vec2 value)
{
	glm::vec2 newValue = (value / m_parts[partIndex].getScale()) + glm::vec2(1.0f);
	scale(partIndex, newValue);
}

void Body::incrScale(glm::vec2 value)
{
	if (!m_parts.empty()) {
		incrScale(0, value);
	}
}

void Body::rotate(unsigned int partIndex, float angle)
{
	if (m_parts[partIndex].m_parentJoint >= 0) {
		rotate(partIndex, angle, m_joints[m_parts[partIndex].m_parentJoint].point);
		m_joints[m_parts[partIndex].m_parentJoint].rotate(angle);
	}
	else {
		m_parts[partIndex].rotate(angle);

		for (auto& j : m_parts[partIndex].m_joints) {
			rotate(m_joints[j].son, angle, m_parts[partIndex].m_shape.getCenter());
			MathUtils::rotatePoint(m_joints[j].point, m_parts[partIndex].m_shape.getCenter(), angle);
		}
	}
}
void Body::rotate(unsigned int partIndex, float angle, glm::vec2 pos)
{
	m_parts[partIndex].rotate(angle, pos);

	for (auto& j : m_parts[partIndex].m_joints) {
		rotate(m_joints[j].son, angle, pos);
		MathUtils::rotatePoint(m_joints[j].point, pos, angle);
	}
}
void Body::scale(unsigned int partIndex, glm::vec2 value)
{
	m_parts[partIndex].scale(value);

	if (m_parts[partIndex].m_parentJoint >= 0) {
		m_joints[m_parts[partIndex].m_parentJoint].point =
			MathUtils::scale(m_joints[m_parts[partIndex].m_parentJoint].point, m_parts[partIndex].m_shape.getCenter(), value);
	}

	for (auto& j : m_parts[partIndex].m_joints) {
		scale(m_joints[j].son, value, m_parts[partIndex].m_shape.getCenter());
		m_joints[j].point = MathUtils::scale(m_joints[j].point, m_parts[partIndex].m_shape.getCenter(), value);
	}
}
void Body::scale(unsigned int partIndex, glm::vec2 value, glm::vec2 pos)
{
	m_parts[partIndex].scale(value, pos);

	for (auto& j : m_parts[partIndex].m_joints) {
		scale(m_joints[j].son, value, pos);
		m_joints[j].point = MathUtils::scale(m_joints[j].point, pos, value);
	}
}
void Body::incrPos(unsigned int partIndex, glm::vec2 offset)
{
	m_parts[partIndex].incrPos(offset);

	if (m_parts[partIndex].m_parentJoint >= 0) {
		m_joints[m_parts[partIndex].m_parentJoint].point += offset;
	}

	for (auto& j : m_parts[partIndex].m_joints) {
		incrPos(m_joints[j].son, offset);
	}
}
void Body::setPos(unsigned int partIndex, glm::vec2 pos)
{
	glm::vec2 iPos = m_parts[partIndex].getPos();
	m_parts[partIndex].setPos(pos);

	for (auto& j : m_parts[partIndex].m_joints){
		glm::vec2 offset = m_parts[m_joints[j].son].getPos() - iPos;
		setPos(m_joints[j].son, pos + offset);
		m_joints[j].point += offset;
	}
}

void Body::initAngle(unsigned int partIndex, float angle)
{
	m_parts[partIndex].rotate(angle);

	for (auto& j : m_parts[partIndex].m_joints) {
		initAngle(m_joints[j].son, angle, m_parts[partIndex].m_shape.getCenter());
	}
}
void Body::initAngle(unsigned int partIndex, float angle, glm::vec2 pos)
{
	m_parts[partIndex].rotate(angle, pos);

	for (auto& j : m_parts[partIndex].m_joints) {
		initAngle(m_joints[j].son, angle, pos);
	}
}
void Body::initScale(unsigned int partIndex, glm::vec2 scale)
{
	m_parts[partIndex].scale(scale);

	for (auto& j : m_parts[partIndex].m_joints) {
		initScale(m_joints[j].son, scale, m_parts[partIndex].m_shape.getCenter());
	}
}
void Body::initScale(unsigned int partIndex, glm::vec2 scale, glm::vec2 pos)
{
	m_parts[partIndex].scale(scale, pos);

	for (auto& j : m_parts[partIndex].m_joints) {
		initScale(m_joints[j].son, scale, pos);
	}
}
void Body::initPos(unsigned int partIndex, glm::vec2 pos)
{
	m_parts[partIndex].setPos(pos);
	glm::vec2 iPos = m_parts[partIndex].getPos();

	for (auto& j : m_parts[partIndex].m_joints) {
		initPos(m_joints[j].son, pos + m_parts[m_joints[j].son].getPos() - iPos);
	}
}
#pragma endregion 

#pragma region VIRTUAL PUBLIC METHODS ---------------------------------------------------------------------------------
void Body::draw(NS2::SpriteBatch& spriteBatch) const
{
	for (auto& p : m_parts) {
		p.draw(spriteBatch);
	}
}

void Body::drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const
{
	if(m_parts.size() == 1) m_parts[0].drawDebug(debugRenderer, color);
	else m_shape.drawDebug(debugRenderer, color);
}

void Body::incrPos(glm::vec2 offset)
{
	if (!m_parts.empty()) {
		incrPos(0, offset);
	}
	m_shape.incrPos(offset);
}

void Body::setPos(glm::vec2 pos)
{
	incrPos(pos - getPos());
}

void Body::setCenter(glm::vec2 center)
{
	incrPos(center - getCenter());
}

void Body::setAngle(float angle)
{
	angle = DegToRad(angle) - getAngleRad();

	if (!m_parts.empty()) {
		rotate(0, angle);

		if (m_shape.getType() == Shape2DType::BOX)
			updateBox();
		else
			m_shape.setAngle(angle, m_parts[0].getShape().getCenter());
	}
	else {
		m_shape.setAngle(angle);
	}
}

void Body::setScale(glm::vec2 value)
{
	value = value / getScale();
	if (!m_parts.empty()) {
		scale(0, value);
	}
}

void Body::rotate(float angle)
{
	if (angle == 0.0f) return;
	angle *= DEGTORAD;

	if (!m_parts.empty()) {
		rotate(0, angle);

		if (m_shape.getType() == Shape2DType::BOX)
			updateBox();
		else
			m_shape.rotate(angle, m_parts[0].getShape().getCenter());
	}
	else {
		m_shape.rotate(angle);
	}
}

void Body::rotate(float angle, glm::vec2 pos)
{
	if (angle == 0.0f) return;
	angle *= DEGTORAD;

	if (!m_parts.empty()) {
		rotate(0, angle, pos);

		if (m_shape.getType() == Shape2DType::BOX)
			updateBox();
		else
			m_shape.rotate(angle, pos);
	}
	else {
		m_shape.rotate(angle, pos);
	}
}

void Body::scale(glm::vec2 value)
{
	if (!m_parts.empty()) {
		scale(0, value);
		m_shape.scale(value, m_parts[0].getShape().getCenter());
	}
	else {
		m_shape.scale(value);
	}
}

const Shape2D* Body::isInsideBody(const glm::vec2& p) const
{
	for (auto& bp : m_parts) {
		if (MathUtils::isPointInsideShape(p, bp.getShape()))
			return &bp.getShape();
	}
	return nullptr;
}

#pragma endregion 




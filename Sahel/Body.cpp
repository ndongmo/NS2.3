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
	scale(m_initScale.x, m_initScale.y);
	setPos(m_initPos.x, m_initPos.y);
}

void Body::reinit()
{
	m_needUpdate = true;

	// order is important!
	setAngle(m_initAngle);
	setScale(m_initScale.x, m_initScale.y);
	setPos(m_initPos.x, m_initPos.y);
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
void Body::setPart(unsigned int index, BodyPart& part)
{
	part.m_index = index;
	m_parts[index] = part;
}

void Body::addAngularImpulse(float angle)
{
	m_angularVelocity += angle;
}
void Body::addImpulse(float x, float y)
{
	m_forces.x += x;
	m_forces.y += y;
}
void Body::applyImpulse(float friction, float deltaTime)
{
	m_forces *= friction;

	incrPos(m_forces.x, m_forces.y);
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
			m_parts[0].getConstShape().computeStaticAABB(aabb);
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
		incrPos(m_forces.x, m_forces.y);

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
		if (p.getConstSprite().depth > depth)
			depth = p.getConstSprite().depth;
	}
	return depth;
}
const glm::vec2& Body::getConstCenter() const
{
	if (m_parts.empty())
		return m_shape.getConstCenter();
	else if (m_parts[0].m_parentJoint >= 0) 
		return  m_joints[m_parts[0].m_parentJoint].point;
	else 
		return m_parts[0].getConstShape().getConstCenter();
}
glm::vec2 Body::getPos() const
{
	return getConstShape().getPos();
}
glm::vec2 Body::getMidleFacePoint() const
{
	return (getConstRealShape().getConstPos(NS2::BR) + getConstRealShape().getConstPos(NS2::TR)) / 2.0f;
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
			for (uint16_t i = 0; i < p.getConstShape().getConstPoints().size(); i++)
				points.push_back(p.getConstShape().getConstPoints().at(i));
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
			center += p.getConstShape().getConstCenter();
		}
	}
	center /= i;

	for (auto& p : m_parts){
		if (p.canCollide()) {
			tmp = p.getConstShape().getFartherDis(center);
			if (farther < tmp)
				farther = tmp;
		}
	}

	m_shape.setCircle(center.x, center.y, farther);
}

void Body::updateBox()
{
	float minX = m_parts[0].getConstShape().getMinX();
	float minY = m_parts[0].getConstShape().getMinY();
	float maxX = m_parts[0].getConstShape().getMaxX();
	float maxY = m_parts[0].getConstShape().getMaxY();
	float x = 0.0f, y = 0.0f;

	for (uint16_t i = 1; i < m_parts.size(); i++) {
		if (m_parts[i].canCollide()) {
			x = m_parts[i].getConstShape().getMinX();
			y = m_parts[i].getConstShape().getMinY();
			if (x < minX) minX = x;
			if (y < minY) minY = y;
			x = m_parts[i].getConstShape().getMaxX();
			y = m_parts[i].getConstShape().getMaxY();
			if (x > maxX) maxX = x;
			if (y > maxY) maxY = y;
		}
	}
	m_shape.setBox(minX, minY, maxX, maxY);
}

void Body::moveX(unsigned int partIndex, float xValue)
{
	float orientation = getAngleRad() + m_orientation;
	float cosAngle = cos(orientation);
	float sinAngle = sin(orientation);

	float y = xValue * sinAngle;
	float x = xValue * cosAngle;
	incrPos(partIndex, x, y);
}
void Body::moveY(unsigned int partIndex, float yValue)
{
	float orientation = getAngleRad() + m_orientation;
	float cosAngle = cos(orientation);
	float sinAngle = sin(orientation);

	float y = yValue * cosAngle;
	float x = -yValue * sinAngle;
	incrPos(partIndex, x, y);
}
void Body::move(unsigned int partIndex, const Move2D& m, const glm::vec2& rotPoint)
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
	incrPos(m.x, m.y);
}
void Body::move(unsigned int jointIndex, float x, float y, float angle)
{
	Move2D m(x, y, angle * DEGTORAD);
	m_joints[jointIndex].correctValue(m);

	float orientation = m_orientation + m_joints[jointIndex].angle + getAngleRad();
	float cosAngle = cos(orientation);
	float sinAngle = sin(orientation);
	float mx = m.x;

	m.a = m.a;
	m.x = mx * cosAngle - m.y * sinAngle;
	m.y = mx * sinAngle + m.y * cosAngle;

	m_joints[jointIndex].point.x += m.x;
	m_joints[jointIndex].point.y += m.y;

	move(m_joints[jointIndex].son, m, m_joints[jointIndex].point);
}
void Body::move(unsigned int jointIndex, const MoveChange& mc)
{
	// scaling and rotating do not work yet!!!
	if ((mc.scaleX != 0 || mc.scaleY != 0) && mc.angle == 0) {
		incrScale(m_joints[jointIndex].son, mc.scaleX, mc.scaleY);
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

void Body::incrScale(unsigned int partIndex, float x, float y)
{
	BodyPart& p = m_parts[partIndex];
	x = (x / p.getScaleX()) + 1;
	y = (y / p.getScaleY()) + 1;
	scale(partIndex, x, y);
}

void Body::incrScale(float x, float y)
{
	if (!m_parts.empty()) {
		incrScale(0, x, y);
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
			rotate(m_joints[j].son, angle, m_parts[partIndex].m_shape.getConstCenter());
			MathUtils::rotatePoint(m_joints[j].point, m_parts[partIndex].m_shape.getConstCenter(), angle);
		}
	}
}
void Body::rotate(unsigned int partIndex, float angle, const glm::vec2& pos)
{
	m_parts[partIndex].rotate(angle, pos);

	for (auto& j : m_parts[partIndex].m_joints) {
		rotate(m_joints[j].son, angle, pos);
		MathUtils::rotatePoint(m_joints[j].point, pos, angle);
	}
}
void Body::scale(unsigned int partIndex, float x, float y)
{
	scaleX(partIndex, x);
	scaleY(partIndex, y);
}
void Body::scaleX(unsigned int partIndex, float x)
{
	m_parts[partIndex].scaleX(x);

	if (m_parts[partIndex].m_parentJoint >= 0) {
		m_joints[m_parts[partIndex].m_parentJoint].point.x =
			MathUtils::scale(m_joints[m_parts[partIndex].m_parentJoint].point.x, m_parts[partIndex].m_shape.getCenterX(), x);
	}

	for (auto& j : m_parts[partIndex].m_joints) {
		scaleX(m_joints[j].son, x, m_parts[partIndex].m_shape.getCenterX());
		m_joints[j].point.x = MathUtils::scale(m_joints[j].point.x, m_parts[partIndex].m_shape.getCenterX(), x);
	}
}
void Body::scaleY(unsigned int partIndex, float y)
{
	m_parts[partIndex].scaleY(y);

	if (m_parts[partIndex].m_parentJoint >= 0) {
		m_joints[m_parts[partIndex].m_parentJoint].point.y =
			MathUtils::scale(m_joints[m_parts[partIndex].m_parentJoint].point.y, m_parts[partIndex].m_shape.getCenterY(), y);
	}

	for (auto& j : m_parts[partIndex].m_joints) {
		scaleY(m_joints[j].son, y, m_parts[partIndex].m_shape.getCenterY());
		m_joints[j].point.y = MathUtils::scale(m_joints[j].point.y, m_parts[partIndex].m_shape.getCenterY(), y);
	}
}
void Body::scaleX(unsigned int partIndex, float x, float posX)
{
	m_parts[partIndex].scaleX(x, posX);

	for (auto& j : m_parts[partIndex].m_joints) {
		scaleX(m_joints[j].son, x, posX);
		m_joints[j].point.x = MathUtils::scale(m_joints[j].point.x, posX, x);
	}
}
void Body::scaleY(unsigned int partIndex, float y, float posY)
{
	m_parts[partIndex].scaleY(y, posY);

	for (auto& j : m_parts[partIndex].m_joints) {
		scaleY(m_joints[j].son, y, posY);
		m_joints[j].point.y = MathUtils::scale(m_joints[j].point.y, posY, y);
	}
}
void Body::incrPosX(unsigned int partIndex, float x)
{
	m_parts[partIndex].incrPosX(x);

	if (m_parts[partIndex].m_parentJoint >= 0) {
		m_joints[m_parts[partIndex].m_parentJoint].point.x += x;
	}

	for (auto& j : m_parts[partIndex].m_joints) {
		incrPosX(m_joints[j].son, x);
	}
}
void Body::incrPosY(unsigned int partIndex, float y)
{
	m_parts[partIndex].incrPosY(y);

	if (m_parts[partIndex].m_parentJoint >= 0) {
		m_joints[m_parts[partIndex].m_parentJoint].point.y += y;
	}

	for (auto& j : m_parts[partIndex].m_joints) {
		incrPosY(m_joints[j].son, y);
	}
}
void Body::incrPos(unsigned int partIndex, float x, float y)
{
	if (x != 0.0f) incrPosX(partIndex, x);
	if (y != 0.0f) incrPosY(partIndex, y);
}
void Body::setPos(unsigned int partIndex, float x, float y)
{
	float posX = m_parts[partIndex].m_drawShape.getPosX();
	float posY = m_parts[partIndex].m_drawShape.getPosY();
	float xOffset = 0.0f;
	float yOffset = 0.0f;

	m_parts[partIndex].setPos(x, y);

	for (auto& j : m_parts[partIndex].m_joints){
		xOffset = m_parts[m_joints[j].son].getPosX() - posX;
		yOffset = m_parts[m_joints[j].son].getPosY() - posY;

		setPos(m_joints[j].son, x + xOffset, y + yOffset);
		m_joints[j].point.x += x + xOffset;
		m_joints[j].point.y += y + yOffset;
	}
}

void Body::initAngle(unsigned int partIndex, float angle)
{
	m_parts[partIndex].rotate(angle);

	for (auto& j : m_parts[partIndex].m_joints) {
		initAngle(m_joints[j].son, angle, m_parts[partIndex].m_shape.getConstCenter());
	}
}
void Body::initAngle(unsigned int partIndex, float angle, const glm::vec2& pos)
{
	m_parts[partIndex].rotate(angle, pos);

	for (auto& j : m_parts[partIndex].m_joints) {
		initAngle(m_joints[j].son, angle, pos);
	}
}
void Body::initScale(unsigned int partIndex, float x, float y)
{
	initScaleX(partIndex, x);
	initScaleY(partIndex, y);
}
void Body::initScaleX(unsigned int partIndex, float x)
{
	m_parts[partIndex].scaleX(x);

	for (auto& j : m_parts[partIndex].m_joints) {
		initScaleX(m_joints[j].son, x, m_parts[partIndex].m_shape.getCenterX());
	}
}
void Body::initScaleY(unsigned int partIndex, float y)
{
	m_parts[partIndex].scaleY(y);

	for (auto& j : m_parts[partIndex].m_joints) {
		initScaleY(m_joints[j].son, y, m_parts[partIndex].m_shape.getCenterY());
	}
}
void Body::initScaleX(unsigned int partIndex, float x, float posX)
{
	m_parts[partIndex].scaleX(x, posX);

	for (auto& j : m_parts[partIndex].m_joints) {
		initScaleX(m_joints[j].son, x, posX);
	}
}
void Body::initScaleY(unsigned int partIndex, float y, float posY)
{
	m_parts[partIndex].scaleY(y, posY);

	for (auto& j : m_parts[partIndex].m_joints) {
		initScaleY(m_joints[j].son, y, posY);
	}
}
void Body::initPos(unsigned int partIndex, float x, float y)
{
	m_parts[partIndex].setPos(x, y);

	float posX = m_parts[partIndex].m_drawShape.getPosX();
	float posY = m_parts[partIndex].m_drawShape.getPosY();
	float xOffset = 0.0f;
	float yOffset = 0.0f;

	for (auto& j : m_parts[partIndex].m_joints) {
		xOffset = m_parts[m_joints[j].son].getPosX() - posX;
		yOffset = m_parts[partIndex].m_shape.getPosY() - posY;

		initPos(m_joints[j].son, x + xOffset, y + yOffset);
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

float Body::getPosX() const
{
	return getConstShape().getPosX();
}
float Body::getPosY() const
{
	return getConstShape().getPosY();
}
void Body::incrPosX(float x)
{
	if (x == 0.0f) return;

	if (!m_parts.empty()) {
		incrPosX(0, x);
	}
	m_shape.incrPosX(x);
}

void Body::incrPosY(float y)
{
	if (y == 0.0f) return;

	if (!m_parts.empty()) {
		incrPosY(0, y);
	}
	m_shape.incrPosY(y);
}

void Body::incrPos(float x, float y)
{
	incrPosX(x);
	incrPosY(y);
}

void Body::setPos(float x, float y)
{
	float xOffset = x - getPosX();
	float yOffset = y - getPosY();

	if (!m_parts.empty()) {
		incrPos(0, xOffset, yOffset);
	}
	m_shape.incrPos(xOffset, yOffset);
}

void Body::setCenter(float x, float y)
{
	float xOffset = x - getConstShape().getCenterX();
	float yOffset = y - getConstShape().getCenterY();

	if (!m_parts.empty()) {
		incrPos(0, xOffset, yOffset);
	}
	m_shape.incrPos(xOffset, yOffset);
}

void Body::setAngle(float angle)
{
	angle = (angle * DEGTORAD) - getAngleRad();

	if (!m_parts.empty()) {
		rotate(0, angle);

		if (m_shape.getType() == Shape2DType::BOX)
			updateBox();
		else
			m_shape.setAngle(angle, m_parts[0].getConstShape().getConstCenter());
	}
	else {
		m_shape.setAngle(angle);
	}
}

void Body::setScale(float x, float y)
{
	x = x / getScaleX();
	y = y / getScaleY();

	if (!m_parts.empty() && (x != 1.0f || y != 1.0f)) {
		scale(0, x, y);
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
			m_shape.rotate(angle, m_parts[0].getConstShape().getConstCenter());
	}
	else {
		m_shape.rotate(angle);
	}
}

void Body::rotate(float angle, const glm::vec2& pos)
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

void Body::scaleX(float x)
{
	if (!m_parts.empty()) {
		scaleX(0, x);
		m_shape.scaleX(x, m_parts[0].getConstShape().getCenterX());
	}
	else {
		m_shape.scaleX(x);
	}
}

void Body::scaleY(float y)
{
	if (!m_parts.empty()) {
		scaleY(0, y);
		m_shape.scaleY(y, m_parts[0].getConstShape().getCenterY());
	}
	else {
		m_shape.scaleY(y);
	}
}

void Body::scale(float x, float y)
{
	scaleX(x);
	scaleY(y);
}

const Shape2D* Body::isInsideBody(const glm::vec2& p) const
{
	for (auto& bp : m_parts) {
		if (MathUtils::isPointInsideShape(p, bp.getConstShape()))
			return &bp.getConstShape();
	}
	return nullptr;
}

#pragma endregion 




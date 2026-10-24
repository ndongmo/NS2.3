#include "MathUtils.h"
#include "Shape2D.h"
#include "Body.h"

#include <algorithm>
#include <cmath>
#include <glm\gtx\rotate_vector.hpp>

#define COLLISION_TOLERANCE	1.5f
#define FACING_FACTOR 0.80f
#define PI2 PI * 2
#define OBSTACLE_TOLERANCE 10.0f

bool MathUtils::avoidObstacle(const Shape2D& src, const Shape2D& obstacle, const glm::vec2& target, glm::vec2& step)
{
	const glm::vec2& srcCenter = src.getConstCenter(), obsCenter = obstacle.getConstCenter();
	const float srcMinX = src.getMinX(), srcMax = src.getMaxX(), srcMinY = src.getMinY(), srcMaxY = src.getMaxY(), 
		obsMinX = obstacle.getMinX(), obsMaxX = obstacle.getMaxX(), obsMinY = obstacle.getMinY(), obsMaxY = obstacle.getMaxY();

	if (srcMinX - obsMaxX >= -OBSTACLE_TOLERANCE || obsMinX - srcMax >= -OBSTACLE_TOLERANCE) { // obstacle is left or right of the src
		const glm::vec2 bottom(srcCenter.x, obsMaxY + src.getDimY());
		const glm::vec2 top(srcCenter.x, obsMinY - src.getDimY());
		step = glm::length(target - top) < glm::length(target - bottom) ? top : bottom;
		return true;
	}
	else if (srcMinY - obsMaxY >= -OBSTACLE_TOLERANCE || obsMinY - srcMaxY >= -OBSTACLE_TOLERANCE) { // obstacle is top or bottom of the src
		const glm::vec2 right(obsMaxX + src.getDimX(), srcCenter.y);
		const glm::vec2 left(obsMinX - src.getDimX(), srcCenter.y);
		step = glm::length(target - right) < glm::length(target - left) ? right : left;
		return true;
	}

	return false;
}

int MathUtils::logarithmGrowth(int min, int max, int maxValue, int x)
{
	if (maxValue <= 0) return 0;

	float result = min + std::log(1 + ((E_NUMBER / maxValue) * x)) * (max - min);
	return (int)result;
}

float MathUtils::computeDamage(float attack, float defence)
{
	float result = 0.0f;

	if (attack < defence) {
		result = (float)(0.4f * (powf(attack, 3) / powf(defence, 2)) - (0.09f * (powf(attack, 2) / defence) + (0.1 * attack)));
	}
	else {
		result = attack - (0.79f * defence * expf(-0.27f * (defence / attack)));
	}
	return result;
}

int MathUtils::myMax(int x1, int x2)
{
	return (x1 > x2) ? x1 : x2;
}
bool MathUtils::isCircleInView(const glm::vec2& center, float radius, const glm::vec4& view)
{
	return isBoxCollide(center.x - radius, center.y - radius, radius * 2, radius * 2, view.x, view.y, view.z, view.w);
}
bool MathUtils::isBoxInView(const Shape2D& box, const glm::vec4& view)
{
	return isBoxCollide(box.getMinX(), box.getMinY(), box.getWidth(), box.getHeight(), view.x, view.y, view.z, view.w);
}
bool MathUtils::isBoxInView(const AABB& box, const glm::vec4& view)
{
	return isBoxCollide(box.x1, box.y1, box.getWidth(), box.getHeight(), view.x, view.y, view.z, view.w);
}
bool MathUtils::isBoxInView(const glm::vec4& box, const glm::vec4& view)
{
	return isBoxCollide(box.x, box.y, box.z, box.w, view.x, view.y, view.z, view.w);
}
bool MathUtils::isBoxInView(float x, float y, float w, float h, const glm::vec4& view)
{
	return isBoxCollide(x, y, w, h, view.x, view.y, view.z, view.w);
}
bool MathUtils::isBoxOutView(const Shape2D& box, const AABB& view)
{
	return box.getMaxX() >= view.x2 || box.getMaxY() >= view.y2 || box.getMinX() <= view.x1 || box.getMinY() <= view.y1;
}
bool MathUtils::isOverlapping(const AABB& a, const AABB& b)
{
	return !(b.x1 > a.x2 || b.y1 > a.y2 || a.x1 > b.x2 || a.y1 > b.y2);
}
void MathUtils::rotatePoint(glm::vec2& point, const glm::vec2& center, float angle)
{
	point -= center;
	point = glm::rotate(point, angle);
	point += center;
}

void MathUtils::rotatePoint(glm::vec2& point, const glm::vec2& center, const glm::vec2& dims, float angle)
{
	point -= center - (dims / 2.0f);
	point = glm::rotate(point, angle);
	point += center - (dims / 2.0f);
}

glm::vec2 MathUtils::rotatePointToMove(const glm::vec2& point, const glm::vec2& center, float angle)
{
	glm::vec2 pos = point;
	pos -= center;
	pos = glm::rotate(pos, angle);
	pos += center;
	pos -= point;
	return pos;
}

bool MathUtils::resize(const glm::vec2& target, const glm::vec2& init, float r, glm::vec4& box)
{
	// bottom horizontal line
	glm::vec2 point1(box.x, box.y);
	glm::vec2 point2(box.x + box.z, box.y);
	glm::vec2 center(box.x + (box.z /2.0f), box.y + (box.w / 2.0f));

	if (MathUtils::isLineCircleCollide(point1, point2, init, r)) {

		float dist = computeDistance(target, init, center);
		box.y -= dist;
		box.w += dist;
		return true;
	}
	// right vertical line
	point1.x += box.z;
	point1.y += box.w;
	if (MathUtils::isLineCircleCollide(point2, point1, init, r)) {

		box.z += computeDistance(target, init, center);
		return true;
	}
	// top horizontal line
	point2.y += box.w;
	point2.x -= box.z;
	if (MathUtils::isLineCircleCollide(point1, point2, init, r)) {

		box.w += computeDistance(target, init, center);
		return true;
	}
	// left vertical line
	point1.x -= box.z;
	point1.y -= box.w;
	if (MathUtils::isLineCircleCollide(point2, point1, init, r)) {

		float dist = computeDistance(target, init, center);
		box.x -= dist;
		box.z += dist;
		return true;
	}

	return false;
}

float MathUtils::computeDistance(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& center)
{
	float dist = glm::distance(point1, point2);
	if (glm::distance(point2, center) > glm::distance(point1, center))
		dist = -dist;
	return dist;
}

glm::vec2 MathUtils::rotatePoint(const glm::vec2& point, float angle)
{
	glm::vec2 newPoint;
	newPoint.x = point.x * cos(angle) - point.y * sin(angle);
	newPoint.y = point.x * sin(angle) + point.y * cos(angle);
	return newPoint;
}

float MathUtils::scale(float toScale, float center, float value)
{
	return center + (toScale - center) * value;
}

float MathUtils::normalizeAngle(float angle)
{
	if (angle > PI2)       return angle - PI2;
	else if (angle < 0) return  PI2 + angle;
	else                  return angle;
}

bool MathUtils::isFacingFront(const Body& target, const Body& body)
{
	Orientation or1 = MathUtils::findOrientation(target.getAngle());
	Orientation or2 = MathUtils::findOrientation(body.getAngle());
	Orientation result = MathUtils::computeOrientation(or1, or2);
	glm::vec2 norm = glm::normalize(body.getConstCenter() - target.getConstCenter());

	return result == Orientation::LEFT &&
		((or1 == Orientation::DOWN && norm.y < -FACING_FACTOR) || (or1 == Orientation::UP && norm.y > FACING_FACTOR) ||
		(or1 == Orientation::LEFT && norm.x < -FACING_FACTOR) || (or1 == Orientation::RIGHT && norm.x > FACING_FACTOR));
}

bool MathUtils::isFacingFrontBack(const Body& target, const Body& body)
{
	Orientation or1 = MathUtils::findOrientation(target.getAngle());
	Orientation or2 = MathUtils::findOrientation(body.getAngle());
	Orientation result = MathUtils::computeOrientation(or1, or2);
	glm::vec2 norm = glm::normalize(body.getConstCenter() - target.getConstCenter());

	return 
		(result == Orientation::LEFT &&
		((or1 == Orientation::DOWN && norm.y < -FACING_FACTOR) || (or1 == Orientation::UP && norm.y > FACING_FACTOR) ||
			(or1 == Orientation::LEFT && norm.x < -FACING_FACTOR) || (or1 == Orientation::RIGHT && norm.x > FACING_FACTOR))) 
		|| 
		(result == Orientation::RIGHT &&
			((or1 == Orientation::DOWN && norm.y > -FACING_FACTOR) || (or1 == Orientation::UP && norm.y < FACING_FACTOR) ||
				(or1 == Orientation::LEFT && norm.x > -FACING_FACTOR) || (or1 == Orientation::RIGHT && norm.x < FACING_FACTOR)));
}

Orientation MathUtils::computeOrientation(Orientation or1, Orientation or2)
{
	or1 = (Orientation)((int)or1 - (int)or2);
	or1 = (Orientation)((or1 == 360) ? 0 : ((or1 == -270) ? 90 :
		((or1 == -180) ? 180 : ((or1 == -90) ? 270 : or1))));

	return or1;
}

Orientation MathUtils::findOrientation(float x, float y)
{
	if (abs(x) >= abs(y)) {
		if (x >= 0.0f) return Orientation::RIGHT;
		else return Orientation::LEFT;
	}
	else if (y >= 0.0f)
		return Orientation::UP;
	else
		return Orientation::DOWN;
}

Orientation MathUtils::findOrientation(float angle)
{
	angle = (float)((int)angle % 360);

	if ((angle <= 30.0f && angle >= -30.0f) || (angle >= 330.0f || angle <= -330.0f))
		return Orientation::RIGHT;
	else if (angle >= 30.0f && angle <= 150.0f || (angle <= -210.0f && angle >= -330.0f))
		return Orientation::UP;
	else if ((angle >= 150.0f && angle <= 210.0f) || (angle <= -150 && angle >= -210))
		return Orientation::LEFT;
	else //if ((angle >= 210.0f && angle <= 330.0f) || (angle <= -30.0f && angle >= -150.0f))
		return Orientation::DOWN;
}

float MathUtils::findOrientation(const Body& body, const glm::vec2& target)
{
	const glm::vec2& norm = glm::normalize(target - body.getConstShape().getConstCenter());
	return calculateAngle(norm, body.getOrientation());
}

float MathUtils::cross(const glm::vec2& o, const glm::vec2& a, const glm::vec2& b)
{
	return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
float MathUtils::cross(const glm::vec2& a, const glm::vec2& b)
{
	return a.x * b.y - a.y * b.x;
}
glm::vec2 MathUtils::cross(float s, const glm::vec2& a)
{
	return glm::vec2(-s * a.y, s * a.x);
}

void MathUtils::findMTD(std::vector<glm::vec2>& pushVector, int iNumVectors, glm::vec2& collisionDepth)
{
	collisionDepth = pushVector[0];
	float mind2 = glm::dot(pushVector[0], pushVector[0]);
	for (int i = 1; i < iNumVectors; i++)
	{
		float d2 = glm::dot(pushVector[i], pushVector[i]);
		if (d2 < mind2)
		{
			mind2 = d2;
			collisionDepth = pushVector[i];
		}
	}
}

bool MathUtils::checkEdgeIntersection(
	const glm::vec2& p0,
	const glm::vec2& p1,
	const glm::vec2& p2,
	const glm::vec2& p3)
{
	float s1_x = p1.x - p0.x;
	float s1_y = p1.y - p0.y;
	float s2_x = p3.x - p2.x;
	float s2_y = p3.y - p2.y;
	float s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
	float t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);
	return (s > 0 && s < 1 && t > 0 && t < 1);
}

bool MathUtils::checkEdgeIntersection(
	const std::vector<glm::vec2>& points,
	const glm::vec2& curEdgeStart,
	const glm::vec2& curEdgeEnd,
	const glm::vec2& checkEdgeStart,
	const glm::vec2& checkEdgeEnd)
{
	for (uint32_t i = 0; i < points.size() - 2; i++)
	{
		if (curEdgeStart == points[i] && curEdgeEnd == points[i + 1]) {
			continue;
		}

		if (checkEdgeIntersection(checkEdgeStart, checkEdgeEnd, points[i], points[i + 1])) {
			return true;
		}
	}
	return false;
}

float MathUtils::distance(
	const glm::vec2& p,
	const glm::vec2& segmentStart,
	const glm::vec2& segmentEnd)
{
	const float l2 = glm::distance(segmentStart, segmentEnd);
	if (l2 == 0.0) {
		return glm::distance(p, segmentStart);   // v == w case
	}

	// Consider the line extending the segment, parameterized as v + t (w - v)
	// We find projection of point p onto the line.
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	const float t = glm::dot((p - segmentStart), (segmentEnd - segmentStart)) / l2;
	if (t < 0.0) {
		return glm::distance(p, segmentStart); // Beyond the 'v' end of the segment
	}
	else if (t > 1.0) {
		return glm::distance(p, segmentEnd);   // Beyond the 'w' end of the segment
	}

	// Projection falls on the segment
	glm::vec2 projection = segmentStart + (segmentEnd - segmentStart) * t;
	return glm::distance(p, projection);
}

glm::vec2 MathUtils::nearestInnerPoint(glm::vec2 edgeStart,
	glm::vec2 edgeEnd,
	const std::vector<glm::vec2>& points,
	const std::vector<glm::vec2>& hull,
	bool* found)
{
	glm::vec2 result;
	float dist = 0;
	*found = false;

	for (auto& p : points) {
		// Skip points that are already in the hull
		if (std::find(hull.begin(), hull.end(), p) != hull.end()) {
			continue;
		}
		/*if (!IsPointInsidePolygon(p, hull)) {
		continue;
		}*/

		float d = distance(p, edgeStart, edgeEnd);
		bool skip = false;
		for (uint32_t i = 0; !skip && i < hull.size() - 1; i++) {
			float dTmp = distance(p, hull[i], hull[i + 1]);
			skip |= dTmp < d;
		}
		if (skip) {
			continue;
		}

		if (!(*found) || dist > d) {
			result = p;
			dist = d;
			*found = true;
		}
	}
	return result;
}

float MathUtils::minDistance(const glm::vec2& start, const std::vector<glm::vec2>& points)
{
	float dst = glm::distance(start, points[0]);
	for (uint32_t i = 1; i < points.size(); i++) {
		float curDistance = glm::distance(start, points[i]);
		if (curDistance < dst) {
			dst = curDistance;
		}
	}
	return dst;
}

std::vector<glm::vec2> MathUtils::convexHull(std::vector<glm::vec2>& points)
{
	size_t n = points.size();
	int k = 0;
	if (n == 1) {
		return points;
	}
	std::vector<glm::vec2> hull(n * 2);

	// Sort points lexicographically
	std::sort(
		points.begin(),
		points.end(),
		[](const glm::vec2& p1, const glm::vec2& p2)
	{ return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y); }
	);

	// Build lower hull
	for (size_t i = 0; i < n; ++i) {
		while (k >= 2 && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) k--;
		hull[k++] = points[i];
	}

	// Build upper hull
	for (size_t i = n - 2, t = k + 1; i >= 0; i--) {
		while (k >= t && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) k--;
		hull[k++] = points[i];
	}

	hull.resize(k - 1);
	return hull;
}

std::vector<glm::vec2> MathUtils::concaveHull(std::vector<glm::vec2>& points, float k)
{
	std::vector<glm::vec2> hull = convexHull(points);

	for (uint32_t i = 0; i < hull.size() - 1; i++) {
		const glm::vec2& ci1 = hull[i];
		const glm::vec2& ci2 = hull[i + 1];

		bool found;
		const glm::vec2& pk = nearestInnerPoint(ci1, ci2, points, hull, &found);
		if (!found || std::find(hull.begin(), hull.end(), pk) != hull.end()) {
			continue;
		}

		float eh = glm::distance(ci1, ci2);  // the lenght of the edge
		std::vector<glm::vec2> tmp;
		tmp.push_back(ci1);
		tmp.push_back(ci2);
		float dd = minDistance(pk, tmp);

		if (eh / dd > k) {
			// Check that new candidate edge will not intersect existing edges.
			bool intersects = checkEdgeIntersection(hull, ci1, ci2, ci1, pk);
			intersects |= checkEdgeIntersection(hull, ci1, ci2, pk, ci2);
			if (!intersects) {
				hull.insert(hull.begin() + i + 1, pk);
				i--;
			}
		}
	}
	return hull;
}

float MathUtils::calculateAngle(const glm::vec2& angularVel)
{
	// Here, we use dot product to compute the angle between 
	// the origine direction and current direction.
	// we now that right and direction vector are normalized
	// formula : a . b = ||a|| ||b|| cos teta, ||a|| = 1 and ||b|| = 1, so ||a|| * ||b|| = 1

	const glm::vec2 right(1.0f, 0.0f);  // Origine direction
	float angle = acos(glm::dot(right, angularVel));

	// acos return result in [0, pi]. So, for the angles great than 180 in degree or pi in radiant,
	// we keep the negative of angle
	if (angularVel.y < 0.0f)
		angle = -angle;

	return angle;
}

float MathUtils::calculateAngle(const glm::vec2& angularVel, float bodyAngle)
{
	const glm::vec2 right(1.0f, 0.0f);  // Origine direction
	float angle = acos(glm::dot(right, angularVel));
	if (angularVel.y < 0.0f) angle = -angle;
	angle = (angle * RADTODEG) - bodyAngle;
	if (angle > 180.0f) angle -= 360.0f;
	else if (angle < -180.0f) angle += 360.0f;

	return angle;
}

bool MathUtils::AABBCollision(const glm::vec4& a, const glm::vec4& b)
{
	return isBoxCollide(a.x, a.y, a.z, a.w, b.x, b.y, b.z, b.w);
}

bool MathUtils::isShapeCollide(const Shape2D& shape1, const Shape2D& shape2, glm::vec2& collisionDepth)
{
	if (shape1.getType() == Shape2DType::CIRCLE && shape2.getType() == Shape2DType::CIRCLE)
		return isCircleCollide(shape1, shape2, collisionDepth);
	else
		return isBoxCollide(shape1, shape2, collisionDepth);
}

bool MathUtils::isShapeCollide(const Shape2D& shape1, const Shape2D& shape2)
{
	if (shape1.getType() == Shape2DType::CIRCLE && shape2.getType() == Shape2DType::CIRCLE)
		return isCircleCollide(shape1, shape2);
	else
		return isBoxCollide(shape1, shape2);
}

bool MathUtils::isShapeCollide(const Shape2D& shape1, const Shape2D& shape2, float depthFactor)
{
	glm::vec2 depth;
	bool flag;

	if (shape1.getType() == Shape2DType::CIRCLE && shape2.getType() == Shape2DType::CIRCLE)
		flag = isCircleCollide(shape1, shape2, depth);
	else
		flag = isBoxCollide(shape1, shape2, depth);
	if (flag) {
		const glm::vec2& dims = shape2.getConstDims();
		flag = abs(depth.x) >= (dims.x * depthFactor) && abs(depth.y) >= (dims.y * depthFactor);
	}
	return flag;
}

bool MathUtils::isCircleCollide(const Shape2D& shape1, const Shape2D& shape2, glm::vec2& collisionDepth)
{
	float minDist = shape1.getHalfWidth() + shape2.getHalfWidth();
	glm::vec2 distVec = shape1.getCenter() - shape2.getCenter();
	float collision = minDist - glm::length(distVec);

	if (collision > COLLISION_TOLERANCE) {
		if (collision == minDist)
			collisionDepth = glm::normalize(glm::vec2(1.0f, 1.0f)) * collision;
		else
			collisionDepth = glm::normalize(distVec) * collision;
		return true;
	}

	return false;
}

bool MathUtils::isCircleCollide(const Shape2D& shape1, const Shape2D& shape2)
{
	float minDist = shape1.getHalfWidth() + shape2.getHalfWidth();
	glm::vec2 distVec = shape1.getCenter() - shape2.getCenter();
	float collision = minDist - glm::length(distVec);

	return collision > COLLISION_TOLERANCE;
}

bool MathUtils::isBoxCollide(const Shape2D& shape1, const Shape2D& shape2, glm::vec2& collisionDepth)
{
	float minDistX = shape1.getHalfWidth() + shape2.getHalfWidth();
	float minDistY = shape1.getHalfHeight() + shape2.getHalfHeight();

	glm::vec2 distVec = shape1.getCenter() - shape2.getCenter();
	float collisionX = minDistX - abs(distVec.x);
	float collisionY = minDistY - abs(distVec.y);

	if (collisionX > COLLISION_TOLERANCE && collisionY > COLLISION_TOLERANCE) {
		if (collisionX < collisionY) {
			collisionDepth.x = (distVec.x < 0) ? -collisionX : collisionX;
		}
		else {
			collisionDepth.y = (distVec.y < 0) ? -collisionY : collisionY;
		}

		return true;
	}

	return false;
}

bool MathUtils::isBoxCollide(const Shape2D& shape1, const Shape2D& shape2)
{
	return isBoxCollide(shape1, shape2, COLLISION_TOLERANCE);
}

bool MathUtils::isBoxCollide(const Shape2D& shape1, const Shape2D& shape2, float tolerance)
{
	float minDistX = shape1.getHalfWidth() + shape2.getHalfWidth();
	float minDistY = shape1.getHalfHeight() + shape2.getHalfHeight();

	glm::vec2 distVec = shape1.getCenter() - shape2.getCenter();
	float collisionX = minDistX - abs(distVec.x);
	float collisionY = minDistY - abs(distVec.y);

	return collisionX > tolerance && collisionY > tolerance;
}

bool MathUtils::isBoxCollide(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	return x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2;
}

bool MathUtils::isPointInsideShape(const glm::vec2& p, const Shape2D& shape)
{
	if (shape.getType() == Shape2DType::CIRCLE) {
		return isPointInsideCircle(p, shape);
	}
	else if (shape.getType() == Shape2DType::BOX) {
		return isPointInsideBox(p, shape);
	}
	else {
		return isPointInsidePolygon(p, shape);
	}
}
bool MathUtils::isPointInsideCircle(const glm::vec2& p, const Shape2D& shape)
{
	return ((shape.getHalfWidth() - glm::length(p - shape.getConstCenter())) > 0.0f);
}
bool MathUtils::isPointInsideCircle(const glm::vec2& p, const glm::vec2& center, float radius)
{
	return ((radius - glm::length(p - center)) > 0.0f);
}
bool MathUtils::isPointInsideAABB(const glm::vec2& p, const AABB& aabb)
{
	return p.x > aabb.x1 && p.x < aabb.x2 && p.y > aabb.y1 && p.y < aabb.y2;
}
bool MathUtils::isShapeInsideShape(const Shape2D& contained, const Shape2D& container)
{
	AABB a, b;
	contained.computeStaticAABB(a);
	container.computeStaticAABB(b);
	return b.contains(a);
}
bool MathUtils::isPointInsideBox(const glm::vec2& p, const Shape2D& shape)
{
	return (
		p.x >= shape.getCenterX() - shape.getHalfWidth() &&	// right of the left edge AND
		p.x <= shape.getCenterX() + shape.getHalfWidth() &&    // left of the right edge AND
		p.y >= shape.getCenterY() - shape.getHalfHeight() &&	// below the top AND
		p.y <= shape.getCenterY() + shape.getHalfHeight()		// above the bottom
		);
}
bool MathUtils::isPointInsideBox(const glm::vec2& p, const glm::vec4& destRect)
{
	return (
		p.x >= destRect.x &&				// right of the left edge AND
		p.x <= destRect.x + destRect.z &&   // left of the right edge AND
		p.y >= destRect.y &&				// below the top AND
		p.y <= destRect.y + destRect.w		// above the bottom
		);
}
bool MathUtils::isPointInsidePolygon(const glm::vec2& p, const Shape2D& shape)
{
	bool result = false;
	const std::vector<glm::vec2>& polygon = shape.getConstPoints();
	size_t j = polygon.size() - 1;

	for (size_t i = 0; i < polygon.size(); i++)
	{
		if ((polygon[i].y < p.y && polygon[j].y > p.y) || (polygon[j].y < p.y && polygon[i].y > p.y))
		{
			if (polygon[i].x + (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) * (polygon[j].x - polygon[i].x) < p.x)
			{
				result = !result;
			}
		}
		j = i;
	}
	return result;
}

bool MathUtils::isPolygonCollide(const Shape2D& poly, const Shape2D& shape)
{
	float minX = poly.getMinX();
	float minY = poly.getMinY();
	float halfWidth = (poly.getMaxX() - minX) / 2.0f;
	float halfHeight = (poly.getMaxY() - minY) / 2.0f;
	float minDistX = halfWidth + shape.getHalfWidth();
	float minDistY = halfHeight + shape.getHalfHeight();

	float collisionX = minDistX - abs(shape.getCenterX() - (minX + halfWidth));
	float collisionY = minDistY - abs(shape.getCenterY() - (minY + halfHeight));

	return (collisionX > COLLISION_TOLERANCE && collisionY > COLLISION_TOLERANCE);
}

bool MathUtils::isPolyPolygonCollide(const Shape2D& p1, const Shape2D& p2)
{
	float minX1 = p1.getMinX();
	float minY1 = p1.getMinY();
	float halfWidth1 = (p1.getMaxX() - minX1) / 2.0f;
	float halfHeight1 = (p1.getMaxY() - minY1) / 2.0f;
	float minX2 = p2.getMinX();
	float minY2 = p2.getMinY();
	float halfWidth2 = (p2.getMaxX() - minX2) / 2.0f;
	float halfHeight2 = (p2.getMaxY() - minY2) / 2.0f;
	float minDistX = halfWidth1 + halfWidth2;
	float minDistY = halfHeight1 + halfHeight2;

	float collisionX = minDistX - abs((minX2 + halfWidth2) - (minX1 + halfWidth1));
	float collisionY = minDistY - abs((minY2 + halfHeight2) - (minY1 + halfHeight1));

	return (collisionX > COLLISION_TOLERANCE && collisionY > COLLISION_TOLERANCE);
}

bool MathUtils::isLineCircleCollide(const glm::vec2& l1, const glm::vec2& l2, const glm::vec2& c, float r)
{
	float ax = l1.x - c.x;
	float ay = l1.y - c.y;
	float bx = l2.x - c.x;
	float by = l2.y - c.y;
	float dx = bx - ax;
	float dy = by - ay;
	float dr_squared = (dx*dx) + (dy*dy);
	float D = ax*by - bx*ay;

	return (r*r) * dr_squared >(D*D) && 
		((c.x + r < fmaxf(l1.x, l2.x) && c.x + r > fminf(l1.x, l2.x)) || 
		(c.y + r < fmaxf(l1.y, l2.y) && c.y + r > fminf(l1.y, l2.y)));
}
bool MathUtils::isCircleCircleCollide(const glm::vec2& c1, const glm::vec2& c2, float r1, float r2)
{
	return (r1 + r2 - glm::length(c1 - c2)) > 0.0f;
}

bool MathUtils::isLineLineCollide(const glm::vec2& a1, const glm::vec2& a2, const glm::vec2& b1, const glm::vec2& b2, glm::vec2& pt)
{
	glm::vec2 b = a2 - a1;
	glm::vec2 d = b2 - b1;
	float bDotDPerp = b.x * d.y - b.y * d.x;

	// if b dot d == 0, it means the lines are parallel so have infinite intersection points
	if (bDotDPerp == 0)
		return false;

	glm::vec2 c = b1 - a1;
	float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
	if (t < 0 || t > 1)
		return false;

	float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
	if (u < 0 || u > 1)
		return false;

	pt = a1 + t * b;

	return true;
}

bool MathUtils::isLineLineCollide(const glm::vec2& a1, const glm::vec2& a2, const glm::vec2& b1, const glm::vec2& b2)
{
	glm::vec2 b = a2 - a1;
	glm::vec2 d = b2 - b1;
	float bDotDPerp = b.x * d.y - b.y * d.x;

	// if b dot d == 0, it means the lines are parallel so have infinite intersection points
	if (bDotDPerp == 0)
		return false;

	glm::vec2 c = b1 - a1;
	float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
	if (t < 0 || t > 1)
		return false;

	float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
	if (u < 0 || u > 1)
		return false;

	return true;
}

bool MathUtils::isLineAABBCollide(const glm::vec2& a1, const glm::vec2& a2, const AABB& aabb, glm::vec2& pt)
{
	glm::vec2 b = a2 - a1;

	if (std::fminf(a1.x, a2.x) >= aabb.x2 || std::fmaxf(a1.x, a2.x) <= aabb.x1 ||
		std::fminf(a1.y, a2.y) >= aabb.y2 || std::fmaxf(a1.y, a2.y) <= aabb.y1) {
		return false; // no intersection possible
	}

	if (b.x > 0.0f) { // left to right
		if (isLineLineCollide(a1, a2, glm::vec2(aabb.x1, aabb.y1), glm::vec2(aabb.x1, aabb.y2), pt)) {
			return true;
		}
	}
	else if (b.x < 0.0f) { // right to left
		if (isLineLineCollide(a1, a2, glm::vec2(aabb.x2, aabb.y1), glm::vec2(aabb.x2, aabb.y2), pt)) {
			return true;
		}
	}

	if (b.y > 0.0f) { // top to bottom
		if (isLineLineCollide(a1, a2, glm::vec2(aabb.x1, aabb.y1), glm::vec2(aabb.x2, aabb.y1), pt)) {
			return true;
		}
	}
	else if (b.y > 0.0f) { // bottom to top
		if (isLineLineCollide(a1, a2, glm::vec2(aabb.x1, aabb.y2), glm::vec2(aabb.x2, aabb.y2), pt)) {
			return true;
		}
	}
	return false;
}

bool MathUtils::isLineAABBCollide(const glm::vec2& a1, const glm::vec2& a2, const AABB& aabb)
{
	glm::vec2 b = a2 - a1;

	if (std::fminf(a1.x, a2.x) >= aabb.x2 || std::fmaxf(a1.x, a2.x) <= aabb.x1 || 
		std::fminf(a1.y, a2.y) >= aabb.y2 || std::fmaxf(a1.y, a2.y) <= aabb.y1) {
		return false; // no intersection possible
	}

	if (b.x > 0.0f) { // left to right
		if (isLineLineCollide(a1, a2, glm::vec2(aabb.x1, aabb.y1), glm::vec2(aabb.x1, aabb.y2))) {
			return true;
		}
	}
	else if (b.x < 0.0f) { // right to left
		if (isLineLineCollide(a1, a2, glm::vec2(aabb.x2, aabb.y1), glm::vec2(aabb.x2, aabb.y2))) {
			return true;
		}
	}

	if (b.y > 0.0f) { // top to bottom
		if (isLineLineCollide(a1, a2, glm::vec2(aabb.x1, aabb.y1), glm::vec2(aabb.x2, aabb.y1))) {
			return true;
		}
	}
	else if (b.y < 0.0f) { // bottom to top
		if (isLineLineCollide(a1, a2, glm::vec2(aabb.x1, aabb.y2), glm::vec2(aabb.x2, aabb.y2))) {
			return true;
		}
	}
	return false;
}

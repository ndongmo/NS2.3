/*!
* \file MathUtils.h
* \brief Contend all math stuff.
* \author Ndongmo Silatsa Fabrice
* \date 05-04-2017
* \version 2.0
*/

#pragma once

#include <vector>
#include <glm\glm.hpp>

#include "Types.h"

#define DEGTORAD 0.0174532925199432957f		// Constant for converting degree to radian (degValue * DEGTORAD) = radValue
#define RADTODEG 57.295779513082320876f		// Constant for converting radian to degree (radValue * DEGTORAD) = degValue
#define PI 3.14159265f
#define E_NUMBER 1.718281828459f

struct AABB;
class Shape2D;
class Body;

class MathUtils
{
public:
	/*!
	 * \brief Find the next point to avoid obstacle when it is possible.
	 * \param src: source shape
	 * \param obstacle: obstacle to avoid
	 * \param target: targeted point
	 * \param step: next point to find
	 * \return true when the next point is found, false otherwise
	 */
	static bool avoidObstacle(const Shape2D& src, const Shape2D& obstacle, const glm::vec2& target, glm::vec2& step);
	/*!
	 * \brief Compute the logarithm grow of the given x proportionally to its given maximum (maxValue).
	 * The min and max are the interval of the computed value.
	 * \param min : minimum value of the result
	 * \param max : maximum value of the result
	 * \param maxValue : Maximum value of the given x value
	 * \param x : value
	 */
	static int logarithmGrowth(int min, int max, int maxValue, int x);
	/*!
	 * \brief Compute the damage received relatively to attack and defence.
	 * \param attack : received attack
	 * \param defence : current defence
	 */
	static float computeDamage(float attack, float defence);
	/*!
	 * \brief Get the max between the given numbers.
	 */
	static int myMax(int x1, int x2);
	/*!
	* \brief Checks if the given circle is in camera view (exceptionally for camera view).
	*/
	static bool isCircleInView(const glm::vec2& center, float radius, const glm::vec4& view);
	/*!
	* \brief Checks if the given AABB box is in camera view (exceptionally for camera view).
	*/
	static bool isBoxInView(const Shape2D& box, const glm::vec4& view);
	/*!
	* \brief Checks if the given AABB box is in camera view (exceptionally for camera view).
	*/
	static bool isBoxInView(const AABB& box, const glm::vec4& view);
	/*!
	* \brief Checks if the given AABB box is in camera view (exceptionally for camera view).
	*/
	static bool isBoxInView(const glm::vec4& box, const glm::vec4& view);
	/*!
	* \brief Checks if the given box is in camera view (exceptionally for camera view).
	*/
	static bool isBoxInView(float x, float y, float w, float h, const glm::vec4& view);
	/*!
	* \brief Checks if the given AABB box is going out of the view.
	*/
	static bool isBoxOutView(const Shape2D& box, const AABB& view);
	/*!
	* \brief Checks for overlapping of two AABB.
	*/
	static bool isOverlapping(const AABB& a, const AABB& b);
	static float scale(float toScale, float center, float value);
	/*!
	 * \brief Normalize the given angle.
	 * \param angle : in radian
	 */
	static float normalizeAngle(float angle);
	/*!
	 * \brief Check if the given body is facing the target body in front.
	 */
	static bool isFacingFront(const Body& target, const Body& body);
	/*!
	 * \brief Check if the given body is facing the target body in front or back.
	 */
	static bool isFacingFrontBack(const Body& target, const Body& body);
	/*!
	 * \brief Compute the result orientation between the given two ones.
	 */
	static Orientation computeOrientation(Orientation or1, Orientation or2);
	/*!
	 * \brief Get the orientation correspondint to the given point
	 * \param pt : 
	 */
	static Orientation findOrientation(float x, float y);
	/*!
	 * \brief Get the orientation correspondint to the given angle
	 * \param angle : in degree
	 */
	static Orientation findOrientation(float angle);
	/*!
	 * \brief Find body orientation towards the given target
	 */
	static float findOrientation(const Body& body, const glm::vec2& target);
	/*!
	 * \brief Resize the given box from the init point to the targeted point.
	 * \param target : targeted point
	 * \param init : init point
	 * \param r : point radius
	 * \param box : the box to resize
	 * \return true if the box has been resized, false otherwise
	 */
	static bool resize(const glm::vec2& target, const glm::vec2& init, float r, glm::vec4& box);
	/*!
	 * \brief Compute the distance between two points. This distance can be negative depending 
	 *	on the given center point.
	 */
	static float computeDistance(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& center);
	/*!
	* \brief Rotates a point around earth axes.
	* \param point : point to rotate
	* \param angle : rotation angle in radian
	*/
	static glm::vec2 rotatePoint(const glm::vec2& point, float angle);
	/*!
	* \brief Rotates a point around a given center.
	* \param point : point to rotate
	* \param center : rotation point
	* \param angle : rotation angle in radian
	*/
	static void rotatePoint(glm::vec2& point, const glm::vec2& center, float angle);
	/*!
	* \brief Rotates a point around a given center.
	* \param point : point to rotate
	* \param center : rotation point
	* \param dims : dimensions of the shape where the point come from
	* \param angle : rotation angle in radian
	*/
	static void rotatePoint(glm::vec2& point, const glm::vec2& center, const glm::vec2& dims, float angle);
	/*!
	* \brief Rotates a point around a given center and return the result as a linear move.
	* \param point : point to rotate
	* \param center : rotation point
	* \param angle : rotation angle in radian
	*/
	static glm::vec2 rotatePointToMove(const glm::vec2& point, const glm::vec2& center, float angle);
	/*!
	* \brief Computes the 2D cross product of OA and OB vectors.
	* \param o : point O
	* \param a : point A
	* \param b : point B
	* \return Returns a positive value, if OAB makes a counter-clockwise turn,
	*	negative for clockwise turn, and zero if the points are collinear.
	*/
	static float cross(const glm::vec2& o, const glm::vec2& a, const glm::vec2& b);
	static float cross(const glm::vec2& a, const glm::vec2& b);
	static glm::vec2 cross(float s, const glm::vec2& a);
	
	/*!
	* \brief Find the minimum translation distance given a set of vectors.
	* ref : http://elancev.name/oliver/2D%20polygon.htm
	* \param pushVector : set of vectors
	* \param iNumVectors : number of axes
	* \param collisionDepth : MTD vector
	* \return true if it is the case
	*/
	static void findMTD(std::vector<glm::vec2>& pushVector, int iNumVectors, glm::vec2& collisionDepth);
	/*!
	* \brief Computes a convex hull given a list of points.
	* \param points : list of random points
	* \return a list of points on the convex hull in counter-clockwise order.
	*	Note: the last point in the returned list is the same as the first one.
	*/
	static std::vector<glm::vec2> convexHull(std::vector<glm::vec2>& points);
	/*!
	* \brief Computes a concave hull given a list of points.
	* \param points : list of random points
	* \param threshold : define the smoothness of concave hull
	* \return a list of points on the concave hull in counter-clockwise order.
	*/
	static std::vector<glm::vec2> concaveHull(std::vector<glm::vec2>& points, float k);
	/*!
	* \brief Calculates the angle given angular velocity.
	* \param angularVel : angular velocity
	* \return angle in radian
	*/
	static float calculateAngle(const glm::vec2& angularVel);
	/*!
	* \brief Calculates the angle in degree given angular velocity.
	* \param angularVel : angular velocity
	* \return angle in degree
	*/
	static float calculateAngle(const glm::vec2& angularVel, float bodyAngle);
	/*!
	* \brief Simple box-box collision.
	*/
	static bool AABBCollision( const glm::vec4& a, const glm::vec4& b);
	/*!
	 * \brief Shape-shape collision (Actually is only manage circle and box shape.
	 */
	static bool isShapeCollide(const Shape2D& shape1, const Shape2D& shape2, glm::vec2& collisionDepth);
	/*!
	 * \brief Shape-shape collision (Actually is only manage circle and box shape.
	 */
	static bool isShapeCollide(const Shape2D& shape1, const Shape2D& shape2);
	/*!
	 * \brief Shape-shape collision with the given depth factor.
	 */
	static bool isShapeCollide(const Shape2D& shape1, const Shape2D& shape2, float depthFactor);
	/*!
	 * \brief Shape-shape collision.
	 */
	static bool isCircleCollide(const Shape2D& shape1, const Shape2D& shape2, glm::vec2& collisionDepth);
	/*!
	 * \brief Shape-shape collision.
	 */
	static bool isCircleCollide(const Shape2D& shape1, const Shape2D& shape2);
	/*!
	 * \brief Shape-shape collision.
	 */
	static bool isBoxCollide(const Shape2D& shape1, const Shape2D& shape2, glm::vec2& collisionDepth);
	/*!
	 * \brief Shape-shape collision.
	 */
	static bool isBoxCollide(const Shape2D& shape1, const Shape2D& shape2);
	/*!
	 * \brief Shape-shape collision.
	 */
	static bool isBoxCollide(const Shape2D& shape1, const Shape2D& shape2, float tolerance);
	/*!
	 * \brief box to box collision.
	 */
	static bool isBoxCollide(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
	/*!
	* \brief Checks if a 2D point is inside a shape.
	* \param p : point to check
	* \param shape : 2D shape
	* \return true if the point is inside the shape
	*/
	static bool isPointInsideShape(const glm::vec2& p, const Shape2D& shape);
	static bool isPointInsideCircle(const glm::vec2& p, const Shape2D& shape);
	static bool isPointInsideBox(const glm::vec2& p, const Shape2D& shape);
	static bool isPointInsideBox(const glm::vec2& p, const glm::vec4& destRect);
	static bool isPointInsidePolygon(const glm::vec2& p, const Shape2D& shape);
	static bool isPointInsideCircle(const glm::vec2& p, const glm::vec2& center, float radius);
	static bool isPointInsideAABB(const glm::vec2& p, const AABB& aabb);
	static bool isShapeInsideShape(const Shape2D& contained, const Shape2D& container);

	static bool isPolygonCollide(const Shape2D& poly, const Shape2D& shape);
	static bool isPolyPolygonCollide(const Shape2D& poly, const Shape2D& shape);
	/*!
	* \brief Checks for line-circle intersection.
	* \param l1 : line start point
	* \param l2 : line end point
	* \param c : circle center
	* \param r : circle radius
	* \return true if the line intersects the circle
	*/
	static bool isLineCircleCollide(const glm::vec2& l1, const glm::vec2& l2, const glm::vec2& c, float r);
	static bool isCircleCircleCollide(const glm::vec2& c1, const glm::vec2& c2, float r1, float r2);
	/*!
	* \brief Checks for line-line intersection.
	* src: https://stackoverflow.com/questions/3746274/line-intersection-with-aabb-rectangle
	* \param a1 : first line start point
	* \param a2 : first line end point
	* \param b1 : second line start
	* \param b2 : second line end
	* \param pt : point d'intersection
	* \return true when both lines are intersecting, false otherwise
	*/
	static bool isLineLineCollide(const glm::vec2& a1, const glm::vec2& a2, const glm::vec2& b1, const glm::vec2& b2, glm::vec2& pt);
	/*!
	* \brief Checks for line-line intersection.
	* src: https://stackoverflow.com/questions/3746274/line-intersection-with-aabb-rectangle
	* \param a1 : first line start point
	* \param a2 : first line end point
	* \param b1 : second line start
	* \param b2 : second line end
	* \return true when both lines are intersecting, false otherwise
	*/
	static bool isLineLineCollide(const glm::vec2& a1, const glm::vec2& a2, const glm::vec2& b1, const glm::vec2& b2);
	/*!
	* \brief Checks for line-aabb intersection.
	* \param a1 : line start point
	* \param a2 : line end point
	* \param aabb : aabb object
	* \param pt : point d'intersection
	* \return true when the line intersects the aabb, false otherwise
	*/
	static bool isLineAABBCollide(const glm::vec2& a1, const glm::vec2& a2, const AABB& aabb, glm::vec2& pt);
	/*!
	* \brief Checks for line-aabb intersection.
	* \param a1 : line start point
	* \param a2 : line end point
	* \param aabb : aabb object
	* \return true when the line intersects the aabb, false otherwise
	*/
	static bool isLineAABBCollide(const glm::vec2& a1, const glm::vec2& a2, const AABB& aabb);

private:
	/*!
	* \brief Gets the minimum distance between a point and the nearest point in a list of points.
	* \param start : start point
	* \param points : list of points
	* \return minimum distance
	*/
	static float minDistance(const glm::vec2& start, const std::vector<glm::vec2>& points);
	/*!
	* \brief Check intersection between two edges.
	* \param p0 : first point of edge1
	* \param p1 : end point of edge1
	* \param p2 : first point of edge2
	* \param p3 : end point of edge2
	* \return true if the checked edge intersects with another
	*/
	static bool checkEdgeIntersection(
		const glm::vec2& p0,
		const glm::vec2& p1,
		const glm::vec2& p2,
		const glm::vec2& p3);
	/*!
	* \brief Check that new candidate edge will not intersect existing edges.
	* \param points : list of points
	* \param curEdgeStart : first point of start edge
	* \param curEdgeEnd : end point of checked edge
	* \param curEdgeStart : first point of checked edge
	* \param curEdgeEnd : end point of start edge
	* \return true if the checked edge intersects with another
	*/
	static bool checkEdgeIntersection(const std::vector<glm::vec2>& points, 
		const glm::vec2& curEdgeStart, 
		const glm::vec2& curEdgeEnd,
		const glm::vec2& checkEdgeStart,
		const glm::vec2& checkEdgeEnd);
	/*!
	* \brief Find the nearest inner point pk belongs to G from the edge (ci1, ci2).
	*/
	static glm::vec2 nearestInnerPoint(glm::vec2 edgeStart, 
		glm::vec2 edgeEnd, 
		const std::vector<glm::vec2>& points, 
		const std::vector<glm::vec2>& hull, 
		bool* found);
	/*!
	* \brief Calculate the distance between a point and an edge.
	*/
	static float distance(
		const glm::vec2& p,
		const glm::vec2& segmentStart,
		const glm::vec2& segmentEnd);
};


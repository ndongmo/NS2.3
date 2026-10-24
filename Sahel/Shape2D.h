/*!
* \file Shape2D.h
* \brief Basic 2D shape.
* \author Ndongmo Silatsa Fabrice
* \date 07-05-2017
* \version 2.0
*/

#pragma once

#include "ITransform.h"
#include "IDraw.h"
#include "Types.h"

#include <CoreTypes.h>
#include <ostream>
#include <vector>

/*!
* \brief Simple AABB bounding box structure.
*/
struct AABB
{
	/*!
	* \brief Gets the perimeter length.
	*/
	float getPerimeter() const
	{
		return 2.0f * ((x2 - x1) + (y2 - y1));
	}
	/*!
	* \brief Combine the current aabb with the given one into the current.
	*/
	void combine(const AABB& aabb)
	{
		if (x1 > aabb.x1) x1 = aabb.x1;
		if (y1 > aabb.y1) y1 = aabb.y1;
		if (x2 < aabb.x2) x2 = aabb.x2;
		if (y2 < aabb.y2) y2 = aabb.y2;
	}
	/*!
	* \brief Combine two aabbs into the current one.
	*/
	void combine(const AABB& aabb1, const AABB& aabb2)
	{
		x1 = (aabb1.x1 > aabb2.x1) ? aabb2.x1 : aabb1.x1;
		y1 = (aabb1.y1 > aabb2.y1) ? aabb2.y1 : aabb1.y1;
		x2 = (aabb1.x2 < aabb2.x2) ? aabb2.x2 : aabb1.x2;
		y2 = (aabb1.y2 < aabb2.y2) ? aabb2.y2 : aabb1.y2;
	}
	/*!
	* \brief Checks if the given aabb is contained in the current one.
	*/
	bool contains(const AABB& aabb)
	{
		return x1 <= aabb.x1 && y1 <= aabb.y1 && x2 >= aabb.x2 && y2 >= aabb.y2;
	}
	bool overlap(const AABB& b)
	{
		return !(b.x1 > x2 || b.y1 > y2 || x1 > b.x2 || y1 > b.y2);
	}
	void getRect(glm::vec4& destRect) const
	{
		destRect.x = x1;
		destRect.y = y1;
		destRect.z = x2 - x1;
		destRect.w = y2 - y1;
	}
	float getWidth()  const { return x2 - x1; }
	float getHeight() const { return y2 - y1; }

	float x1 = 0.0f, y1 = 0.0f; // lower vertex
	float x2 = 0.0f, y2 = 0.0f; // upper vertex
};

class ObjectLoader;
class ObjectSaver;
class PartEditor;
class Body;

class Shape2D : public ITransform, public IDraw
{
	friend ObjectLoader;
	friend ObjectSaver;
	friend PartEditor;
	friend Body;

public:
	Shape2D();
	/*!
	* \brief Initialize the shape.
	*/
	void init(Shape2DType type);
	/*!
	* \brief Initialize the shape.
	*/
	void init(Shape2DType type, float x, float y, float width, float height);
	/*!
	* \brief Initialize the shape as a circle.
	* \param x : x coordinate
	* \param y : y coordinate
	* \param radius : circle radius
	*/
	void initCircle(float x, float y, float radius);
	/*!
	* \brief Initialize the shape as a box.
	* \param x : x coordinate
	* \param y : y coordinate
	* \param width : width of the box
	* \param height : height of the box
	*/
	void initBox(float x, float y, float width, float height);
	/*!
	* \brief Initialize the shape as a rotated box able to rotate.
	* \param x : x coordinate
	* \param y : y coordinate
	* \param width : width of the box
	* \param height : height of the box
	* \param angle : initial angle
	*/
	void initRotatedBox(float x, float y, float width, float height, float angle);
	/*!
	* \brief Initialize the shape as a polygon.
	* \param points : set of points
	* \param angle : initial angle
	*/
	void initPolygon(const std::vector<glm::vec2>& points, float angle);
	/*!
	* \brief Initialize a rotating triangle shape with center as rotating point.
	* \param x1 : x1 coordinate
	* \param y1 : y1 coordinate
	* \param x2 : x2 coordinate
	* \param y2 : y2 coordinate
	* \param x3 : x3 coordinate
	* \param y3 : y3 coordinate
	* \param angle : initial angle
	*/
	void initTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float angle);
	/*!
	* \brief Create a box.
	* \param x : x coordinate
	* \param y : y coordinate
	* \param width : width of the box
	* \param height : height of the box
	*/
	void createBox(float x, float y, float width, float height);
	/*!
	 * \brief Computes surrounding box for dynamic shape.
	 * It builds the surrounding box with double width and height.
	 */
	void computeDynamicAABB(AABB& aabb) const;
	/*!
	 * \brief Computes surrounding box for static shape.
	 * It builds the surrounding box with the same width and height.
	 */
	void computeStaticAABB(AABB& aabb) const;
	/*!
	 * Update the given aabb so that it is contained in this aabb.
	 */
	void updateAABB(AABB& aabb) const;
	/*!
	 * Update the given vector with the current coordinates.
	 */
	void updateDestRect(glm::vec4& destRect) const;
	/*! 
	 * \brief Reset the scale to 1.0f.
	 */
	void resetScale() { m_scale.x = 1.0f; m_scale.y = 1.0f; }
	/*!
	* \brief Overload of << operator.
	* \param os : output stream
	* \param s : 2D shape
	* \return the output stream with the shape description
	*/
	friend std::ostream& operator<<(std::ostream& os, const Shape2D& s);
	/*!
	* \brief Draw the shape.
	* \param debugRenderer : Renderer in debug mode
	* \param color : Rendering color
	*/
	virtual void drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const override;
	/*!
	* \brief get x position.
	*/
	virtual float getPosX() const override;
	/*!
	* \brief get y position.
	*/
	virtual float getPosY() const override;
	/*!
	* \brief get scale x.
	*/
	virtual float getScaleX() const override { return m_scale.x; }
	/*!
	* \brief get scale y.
	*/
	virtual float getScaleY() const override { return m_scale.y; }
	/*!
	* \brief get angle.
	*/
	virtual float getAngle() const override { return m_angle; }
	/*!
	* \brief Rotate the shape around his rotation point.
	* \param angle : rotation angle in radian
	*/
	virtual void rotate(float angle) override;
	/*!
	* \brief Rotate the shape around a given rotation point.
	* \param angle : rotation angle in radian
	* \param pos : rotation point
	*/
	virtual void rotate(float angle, const glm::vec2& pos) override;
	/*!
	* \brief Scale the shape along the x axis.
	* \param x : x factor
	*/
	virtual void scaleX(float x) override;
	/*!
	* \brief Scale the shape along the y axis.
	* \param y : y factor
	*/
	virtual void scaleY(float y) override;
	/*!
	* \brief Scale the shape along the x and y axes.
	* \param x : x factor
	* \param y : y factor
	*/
	virtual void scale(float x, float y) override;
	/*!
	* \brief Scale the shape along the x axis.
	* \param x : x factor
	* \param posX : x coordinate of scale axis
	*/
	virtual void scaleX(float x, float posX) override;
	/*!
	* \brief Scale the shape along the y axis.
	* \param y : y factor
	* \param posY : y coordinate of scale axis
	*/
	virtual void scaleY(float y, float posY) override;
	/*!
	* \brief Scale the shape along the x and y axes.
	* \param x : x factor
	* \param y : y factor
	* \param posX : x coordinate of scale axis
	* \param posY : y coordinate of scale axis
	*/
	virtual void scale(float x, float y, float posX, float posY) override;
	/*!
	* \brief Increment or decrement the x coordinate.
	* \param x : x value
	*/
	virtual void incrPosX(float x) override;
	/*!
	* \brief Increment or decrement the y coordinate.
	* \param y : y value
	*/
	virtual void incrPosY(float y) override;
	/*!
	* \brief Increment or decrement the x and y coordinates.
	* \param x : x value
	* \param y : y value
	*/
	virtual void incrPos(float x, float y) override;
	/*!
	* \brief Set the x and y coordinates.
	* \param x : x value
	* \param y : y value
	*/
	virtual void setPos(float x, float y) override;
	/*!
	* \brief Move the shape center to the given position.
	* \param x : x value
	* \param y : y value
	*/
	virtual void setCenter(float x, float y) override;
	/*!
	* \brief Set angle.
	* \param angle : in radian
	*/
	virtual void setAngle(float angle) override;
	/*!
	* \brief Set angle around a given rotation point.
	* \param angle : in radian
	* \param pos : rotation point
	*/
	virtual void setAngle(float angle, const glm::vec2& pos) override;

	/* SETTERS */
	void setCircle(float centerX, float centerY, float radius);
	void setBox(float minX, float minY, float maxX, float maxY);
	void setWidth(float width);
	void setHeight(float height);
	void setPosX(float x);
	void setPosY(float y);

	/* GETTERS */
	const glm::vec2& getConstDims() const { return m_dimensions; }
	const glm::vec2& getConstScale() const { return m_scale; }
	const glm::vec2& getConstCenter() const { return m_center; }
	const glm::vec2& getConstPos(unsigned int index) const { return m_points.at(index); }
	const std::vector<glm::vec2>& getConstPoints() const { return m_points; }

	glm::vec2 getLastPoint() const { return m_points.back(); }
	glm::vec2 getDims() const { return m_dimensions; }
	glm::vec2 getHalfDims() const { return m_dimensions / 2.0f; }
	glm::vec2 getCenter() const { return m_center; }
	glm::vec2 getPos() const;

	float getDimX() const { return m_dimensions.x; }
	float getDimY() const { return m_dimensions.y; }
	
	float getCenterX() const { return m_center.x; }
	float getCenterY() const { return m_center.y; }
	float getHalfWidth() const { return (m_dimensions.x / 2.0f); }
	float getHalfHeight() const { return (m_dimensions.y / 2.0f); }
	float getWidth() const { return m_dimensions.x; }
	float getHeight() const { return m_dimensions.y; }
	float getMinX() const;
	float getMinY() const;
	float getMaxX() const;
	float getMaxY() const;
	void getBorders(float& minX, float& minY, float& maxX, float& maxY) const;
	float getFartherDis(const glm::vec2& center) const;
	Shape2DType getType() const{ return m_type; }

private:
	/*!
	* \brief Adds a given point to the shape (only for polygon).
	* \param x : x value
	* \param y : y value
	*/
	bool addPoint(float x, float y);
	/*!
	* \brief Deletes a given point to the shape (only for polygon).
	* \param pt : point index to remove
	*/
	bool deletePoint(unsigned int pt);
	/*!
	* \brief Sets shape type (only in edition).
	* \param newType : new type of the shape
	*/
	void setType(Shape2DType newType);
	/*!
	* \brief Checks if the given circle is on the shape border and resizes the shape to the given point.
	* \param target : new target width and height
	* \param init : init point (circle center)
	* \param r : circle radius
	*/
	bool resize(const glm::vec2& target, const glm::vec2& init, float r);
	/*!
	* \brief Moves the given points perpendicularly.
	* \param p1 : index of the first point
	* \param p2 : index of the second point
	* \param dist : distance to move
	*/
	void move(unsigned int p1, unsigned int p2, float dist);
	/*!
	* \brief computes the distance between 2 points regarding the center.
	*/
	float computeDist(const glm::vec2& target, const glm::vec2& init);
	/*!
	* \brief Sets x coordinate of a given point (only in edition).
	* \param index : point index
	* \param pt : new point location
	*/
	void setPointX(unsigned int index, const glm::vec2& pt);
	void setPointY(unsigned int index, const glm::vec2& pt);
	/*!
	 * \brief Updates the shape dimensions.
	 */
	void updateDimensions();

	glm::vec2 getCentroid() const;
	const glm::vec2& getConstBL() const { return m_points[NS2::BL]; }
	const glm::vec2& getConstTL() const { return m_points[NS2::TL]; }
	const glm::vec2& getConstTR() const { return m_points[NS2::TR]; }
	const glm::vec2& getConstBR() const { return m_points[NS2::BR]; }
	glm::vec2& getBL() { return m_points[NS2::BL]; }
	glm::vec2& getTL() { return m_points[NS2::TL]; }
	glm::vec2& getTR() { return m_points[NS2::TR]; }
	glm::vec2& getBR() { return m_points[NS2::BR]; }
	float getLX() const { return m_points[NS2::BL].x; }
	float getRX() const { return m_points[NS2::BL].x + m_dimensions.x; }
	float getBY() const { return m_points[NS2::BL].y; }
	float getTY() const { return m_points[NS2::BL].y + m_dimensions.y; }

	float m_angle = 0.0f;				/* Angle */
	glm::vec2 m_scale;					/* Scale */
	glm::vec2 m_center;					/* Center point in earth coordinates */
	glm::vec2 m_dimensions;				/* Dimensions */
	Shape2DType m_type;					/* Shape type */
	std::vector<glm::vec2> m_points;	/* Set of points in earth coordinates */
};


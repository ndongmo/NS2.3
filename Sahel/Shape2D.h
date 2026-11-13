/*!
* \file Shape2D.h
* \brief Basic 2D shape.
* \author Ndongmo Silatsa Fabrice
* \date 07-05-2017
* \version 2.1
*/

#pragma once

#include "ITransform.h"
#include "IDraw.h"
#include "Types.h"

#include <CoreTypes.h>
#include <ostream>
#include <vector>
#include <algorithm>

constexpr size_t POLY_MAX_POINTS = 8;
constexpr size_t POLY_MIN_POINTS = 4;

/*!
* \brief Simple AABB bounding box structure.
*/
struct AABB
{
	float getPerimeter() const
	{
		return 2.0f * ((x2 - x1) + (y2 - y1));
	}

	void combine(const AABB& aabb)
	{
		if (x1 > aabb.x1) x1 = aabb.x1;
		if (y1 > aabb.y1) y1 = aabb.y1;
		if (x2 < aabb.x2) x2 = aabb.x2;
		if (y2 < aabb.y2) y2 = aabb.y2;
	}

	void combine(const AABB& aabb1, const AABB& aabb2)
	{
		x1 = (aabb1.x1 > aabb2.x1) ? aabb2.x1 : aabb1.x1;
		y1 = (aabb1.y1 > aabb2.y1) ? aabb2.y1 : aabb1.y1;
		x2 = (aabb1.x2 < aabb2.x2) ? aabb2.x2 : aabb1.x2;
		y2 = (aabb1.y2 < aabb2.y2) ? aabb2.y2 : aabb1.y2;
	}

	bool contains(const AABB& aabb) const
	{
		return x1 <= aabb.x1 && y1 <= aabb.y1 && x2 >= aabb.x2 && y2 >= aabb.y2;
	}

	bool overlap(const AABB& b) const
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

	float x1 = 0.0f, y1 = 0.0f;
	float x2 = 0.0f, y2 = 0.0f;
};

class ObjectLoader;
class ObjectSaver;
class PartEditor;
class BodyEditor;
class ObjectManager;
class Body;

class Shape2D : public ITransform, public IDraw
{
	friend ObjectLoader;
	friend ObjectSaver;
	friend PartEditor;
	friend BodyEditor;
	friend ObjectManager;
	friend Body;

public:
	Shape2D();

	void init(Shape2DType type);
	void init(Shape2DType type, float x, float y, float width, float height);
	void initCircle(float x, float y, float radius);
	void initBox(float x, float y, float width, float height);
	void initRotatedBox(float x, float y, float width, float height, float angle);

	// Refactored to accept vectors by-value to efficiently handle std::move
	void initPolygon(std::vector<glm::vec2> points, float angle);
	void initTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float angle);
	void createBox(float x, float y, float width, float height);

	void computeDynamicAABB(AABB& aabb) const;
	void computeStaticAABB(AABB& aabb) const;
	void updateAABB(AABB& aabb) const;

	friend std::ostream& operator<<(std::ostream& os, const Shape2D& s);
	virtual void drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const override;

	virtual glm::vec2 getPos() const override;
	virtual glm::vec2 getScale() const override { return m_scale; }
	virtual float getAngle() const override { return m_angle; }

	virtual void rotate(float angle) override;
	virtual void rotate(float angle, glm::vec2 pos) override;
	virtual void scale(glm::vec2 scale) override;
	virtual void scale(glm::vec2 scale, glm::vec2 pos) override;

	virtual void incrPos(glm::vec2 pos) override;
	virtual void setPos(glm::vec2 pos) override;
	virtual void setCenter(glm::vec2 center) override;
	virtual void setAngle(float angle) override;
	virtual void setAngle(float angle, glm::vec2 pos) override;

	/* SETTERS */
	void setCircle(float centerX, float centerY, float radius);
	void setBox(float minX, float minY, float maxX, float maxY);
	void setWidth(float width);
	void setHeight(float height);

	/* GETTERS */
	glm::vec2 getDims() const { return m_dimensions; }
	glm::vec2 getCenter() const { return m_center; }
	glm::vec2 getPos(unsigned int index) const { return m_points.at(index); }
	const std::vector<glm::vec2>& getPoints() const { return m_points; }

	glm::vec2 getLastPoint() const { return m_points.back(); }
	glm::vec2 getHalfDims() const { return m_dimensions * 0.5f; }

	float getDimX() const { return m_dimensions.x; }
	float getDimY() const { return m_dimensions.y; }
	float getCenterX() const { return m_center.x; }
	float getCenterY() const { return m_center.y; }
	float getHalfWidth() const { return m_dimensions.x * 0.5f; }
	float getHalfHeight() const { return m_dimensions.y * 0.5f; }
	float getWidth() const { return m_dimensions.x; }
	float getHeight() const { return m_dimensions.y; }

	float getMinX() const;
	float getMinY() const;
	float getMaxX() const;
	float getMaxY() const;
	void getBorders(float& minX, float& minY, float& maxX, float& maxY) const;
	float getFartherDis(const glm::vec2& center) const;
	Shape2DType getType() const { return m_type; }

private:
	bool addPoint(float x, float y);
	bool deletePoint(unsigned int pt);
	void setType(Shape2DType newType);
	bool resize(const glm::vec2& target, const glm::vec2& init, float r);
	void move(unsigned int p1, unsigned int p2, float dist);
	float computeDist(const glm::vec2& target, const glm::vec2& init);
	void setPoint(unsigned int index, glm::vec2 point);
	void updateDimensions();

	glm::vec2 getCentroid() const;
	float getLX() const { return m_points[NS2::BL].x; }
	float getRX() const { return m_points[NS2::BL].x + m_dimensions.x; }
	float getBY() const { return m_points[NS2::BL].y; }
	float getTY() const { return m_points[NS2::BL].y + m_dimensions.y; }

	void getPointsMinMax(float& minX, float& minY, float& maxX, float& maxY) const;

	float m_angle = 0.0f;
	glm::vec2 m_scale;
	glm::vec2 m_center;
	glm::vec2 m_dimensions;
	Shape2DType m_type;
	std::vector<glm::vec2> m_points;
};

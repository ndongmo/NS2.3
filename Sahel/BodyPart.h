/*!
* \file BodyPart.h
* \brief Part of a physical body.
* \author Ndongmo Silatsa Fabrice
* \date 05-04-2017
* \version 2.0
*/

#pragma once

#include "Shape2D.h"
#include "Joint2D.h"
#include "ILinkedBody.h"
#include "IFileManager.h"

#include <TileSheet.h>

class Body;
class PartEditor;
class BodyEditor;
class ObjectLoader;
class ObjectSaver;

/*!
* \brief BodyPart class.
*/
class BodyPart : public ITransform, public IDraw, public ILinkedBody, public IFileManager
{
	friend Body;
	friend PartEditor;
	friend BodyEditor;
	friend ObjectLoader;
	friend ObjectSaver;

public:
	/*!
	* \brief Overload of << operator.
	* \param os : output stream
	* \param p : body part
	* \return the output stream with the body part description
	*/
	friend std::ostream& operator<<(std::ostream& os, const BodyPart& p);
	/*!
	* \brief Draw the body part.
	* \param spriteBatch : batch for drawing
	*/
	virtual void draw(NS2::SpriteBatch& spriteBatch) const override;
	/*!
	* \brief Draw the body part in debug mode.
	* \param debugRender : renderer for debug
	* \param color : color
	*/
	virtual void drawDebug(NS2::DebugRenderer& debugRender, const NS2::ColorRGBA8& color) const override;

	/*!
	 * \brief Increase color values.
	 */
	void incrColor(int16_t r, int16_t g, int16_t b, int16_t a);

	/* SETTERS */
	void setParentJoint(unsigned int jointIndex) { m_parentJoint = jointIndex; }
	void setTexture(NS2::GLTexture* text) { m_sprite.texture = text; }
	void setHidden(bool hidden) { m_hidden = hidden; }

	/* GETTERS */
	bool canCollide() const { return m_collide; }
	bool isScalable() const { return m_scalable; }
	bool isHidden() const { return m_hidden; }
	int getIndex() const { return m_index; }
	MaterialType getMaterial() const { return m_material; }
	int getParentJoint() const { return m_parentJoint; }
	NS2::TileSheet& getSprite() { return m_sprite; }
	const NS2::TileSheet& getConstSprite() const { return m_sprite; }
	const Shape2D& getConstShape()const { return m_shape; }
	const Shape2D& getConstDrawShape()const { return m_drawShape; }
	const std::vector<unsigned int>& getJoints(){ return m_joints; }
	Shape2D& getDrawShape() { return m_drawShape; }
	Shape2D& getShape() { return m_shape; }

private:
	/*!
	* \brief Add a bounded joint.
	* \param jointIndex : joint index
	*/
	void addJoint(unsigned int jointIndex);
	virtual float getPosX() const override;
	virtual float getPosY() const override;
	virtual float getScaleX() const override;
	virtual float getScaleY() const override;
	virtual float getAngle() const override;
	virtual void move(const Move2D& m, const glm::vec2& pos) override;
	virtual void rotate(float angle) override;
	virtual void rotate(float angle, const glm::vec2& pos) override;
	virtual void scaleX(float x) override;
	virtual void scaleY(float y) override;
	virtual void scale(float x, float y) override;
	virtual void incrPosX(float x) override;
	virtual void incrPosY(float y) override;
	virtual void incrPos(float x, float y) override;
	virtual void setPos(float x, float y) override;
	virtual void setCenter(float x, float y) override;
	virtual void setAngle(float angle) override;
	virtual void setAngle(float angle, const glm::vec2& pos) override;
	virtual void scaleX(float x, float posX) override;
	virtual void scaleY(float y, float posY) override;
	virtual void scale(float x, float y, float posX, float posY) override;

	bool m_hidden = false;					/* Informs if the part should be hidden */
	bool m_collide = true;					/* Informs if the part can collide */
	bool m_scalable = false;				/* Informs if the tile sheet can be scaled */
	int m_index = 0;						/* Index in the body */
	int m_parentJoint = -1;					/* Index of the parent joint */
	MaterialType m_material = MaterialType::NONE;		/* Part material type */
	Shape2D m_shape;						/* Collision shape */
	Shape2D m_drawShape;					/* Draw Shape of the body part */
	NS2::TileSheet m_sprite;				/* Part sprite */
	std::vector<unsigned int> m_joints;		/* All related parts */
};
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
class ObjectManager;

/*!
* \brief BodyPart class.
*/
class Part : public ITransform, public IDraw, public ILinkedBody, public IFileManager
{
	friend Body;
	friend PartEditor;
	friend BodyEditor;
	friend ObjectLoader;
	friend ObjectSaver;
	friend ObjectManager;
public:
	/*!
	* \brief Overload of << operator.
	* \param os : output stream
	* \param p : body part
	* \return the output stream with the body part description
	*/
	friend std::ostream& operator<<(std::ostream& os, const Part& p);
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
	 * \brief Initialize the body part.
	 * \param type : type of the part
	 * \param texturePath : path to the texture
	 */
	void init(PartType type, int index, int oppositePart, NS2::GLTexture* texture);
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
	PartType getType() const { return m_type; }
	MaterialType getMaterial() const { return m_material; }
	int getParentJoint() const { return m_parentJoint; }
	const NS2::TileSheet& getSprite() const { return m_sprite; }
	const Shape2D& getShape()const { return m_shape; }
	const Shape2D& getDrawShape()const { return m_drawShape; }
	const std::vector<unsigned int>& getJoints(){ return m_joints; }

private:
	/*!
	* \brief Add a bounded joint.
	* \param jointIndex : joint index
	*/
	void addJoint(unsigned int jointIndex);
	virtual glm::vec2 getPos() const override;
	virtual glm::vec2 getScale() const override;
	virtual float getAngle() const override;
	virtual void move(const Move2D& m, glm::vec2 pos) override;
	virtual void rotate(float angle) override;
	virtual void rotate(float angle, glm::vec2 pos) override;
	virtual void scale(glm::vec2 scale) override;
	virtual void incrPos(glm::vec2 pos) override;
	virtual void incrScale(glm::vec2 scale) override;
	virtual void setPos(glm::vec2 pos) override;
	virtual void setScale(glm::vec2 scale) override;
	virtual void setCenter(glm::vec2 center) override;
	virtual void setAngle(float angle) override;
	virtual void setAngle(float angle, glm::vec2 pos) override;
	virtual void scale(glm::vec2 scale, glm::vec2 pos) override;

	bool m_hidden = false;					/* Informs if the part should be hidden */
	bool m_collide = true;					/* Informs if the part can collide */
	bool m_scalable = false;				/* Informs if the tile sheet can be scaled */
	int m_index = -1;						/* Index of the part in the body */
	int m_parentJoint = -1;					/* Index of the parent joint */
	int m_oppositePart = -1;				/* Index of the opposite part */
	PartType m_type = PartType::PART_HEAD;	/* Type of the part */
	MaterialType m_material = MaterialType::ORGANIC;		/* Part material type */
	Shape2D m_shape;						/* Collision shape */
	Shape2D m_drawShape;					/* Draw Shape of the body part */
	NS2::TileSheet m_sprite;				/* Part sprite */
	std::vector<unsigned int> m_joints;		/* All related parts */
};
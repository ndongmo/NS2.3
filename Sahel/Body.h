/*!
* \file Body.h
* \brief Physical body.
* \author Ndongmo Silatsa F.
* \date 05-04-2017
* \version 2.0
*/

#pragma once

#include "Part.h"
#include "IFileManager.h"
#include "MoveChange.h"

class ObjectLoader;
class ObjectSaver;
class BodyEditor;
class ObjectManager;

#define SPEED_FACTOR 0.006f

class Body : public ITransform, public IDraw, public IFileManager
{
	friend ObjectLoader;
	friend ObjectSaver;
	friend BodyEditor;
	friend ObjectManager;

public:
	
#pragma region
	/*!
	* \brief Initialize the body position, scale and orientation.
	* \param posX : x position of the body
	* \param posY : y position of the body
	* \param scaleX : scale x factor
	* \param scaleY : scale y factor
	* \param angle : body angle
	*/
	void init(float posX, float posY, float scaleX, float scaleY, float angle);

	/*!
	* \brief Reinitialize the body position, scale and orientation with the initial values.
	*/
	void reinit();

	/*!
	* \brief Overload of << operator.
	* \param os : output stream
	* \param b : body
	* \return the output stream with the body description
	*/
	friend std::ostream& operator<<(std::ostream& os, const Body& b);
	/*!
	* \brief Update the surronding shape of the body.
	*/
	void updateShape();
	/*!
	* \brief Update the surronding shape of the body after an annimation.
	* \param angle : angle of the move annimation
	*/
	void updateShape(float angle);
	/*!
	* \brief Replace the part corresponding to the given part.
	*/
	void setPart(unsigned int index, Part& part);
	/*!
	* \brief Add an angular impulse with the given angle value.
	*/
	void addAngularImpulse(float angle);
	/*!
	* \brief add an impulse with the given velocity values.
	*/
	void addImpulse(glm::vec2 impulse);
	/*!
	* \brief Applied the cumulated impulse.
	* \param friction :  friction
	* \param deltaTime : elapsed time
	*/
	void applyImpulse(float friction, float deltaTime);
	/*!
	* \brief Attachs a given movable body to the current body at the given body part.
	* \param body : the body to attach
	*/
	void attach(ITransform* body, unsigned int partIndex = 0);
	/*!
	* \brief Detachs a given movable body attached to the current body at the given body part.
	* \param body : the body to detach
	*/
	void detach(ITransform* body, unsigned int partIndex = 0);
	/*!
	* \brief Detachs all movable bodies attached to the current body.
	*/
	void detachAll();
	/*!
	* \brief Updates the AABB rectangle.
	*/
	void updateAABB(AABB& aabb, EntityState state);
	/*!
	* \brief Returns true if the body has passed one of the boundaries of the AABB.
	*/
	bool needToUpdate(AABB& aabb) const;
	/*!
	* \brief Applies the accumulated forces.
	*/
	void applyForces(float deltaTime);
	/*!
	* \brief Check if a point is inside body.
	* \param p : point to check
	* \return the pointer of the shape of the body part that the point is inside
	*/
	const Shape2D* isInsideBody(const glm::vec2& p) const;

	/*!
	* \brief Apply the given move from the given joint.
	*	Sets the given value if it is greater than upper or lower than lower value.
	* \param x : move x value
	* \param y : move y value
	* \param a : move angle value
	*/
	void move(unsigned int jointIndex, float x, float y, float angle);
	void move(unsigned int jointIndex, const MoveChange& m);
	/*!
	 * \brief Increment the color of the given part with the given values.
	 */
	void incrColor(unsigned int partIndex, int16_t r, int16_t g, int16_t b, int16_t a);
	/*!
	 * \brief Increment the body's color with the given values.
	 */
	void incrColor(int16_t r, int16_t g, int16_t b, int16_t a);
	/*!
	 * \brief Increment the current body'part scale.
	 */
	void incrScale(unsigned int partIndex, glm::vec2 scale);
	/*!
	 * \brief Increment the current body scale.
	 */
	void incrScale(glm::vec2 scale);
	/*!
	 * \brief Apply the given move to the body.
	 */
	void move(const Move2D& m);
	void incrPos(unsigned int partIndex, glm::vec2 offset);

#pragma endregion PUBLIC CLASS METHODS ---------------------------------------------------------------------------------

#pragma region
	virtual void draw(NS2::SpriteBatch& spriteBatch) const override;
	virtual void drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const override;
	virtual glm::vec2 getPos() const override;
	virtual glm::vec2 getScale() const override {
		if (!m_parts.empty())	return m_parts[0].getShape().getScale();
		else					return m_shape.getScale();
	}
	virtual float getAngle() const override {
		if (!m_parts.empty()) {
			if (m_parts[0].m_parentJoint >= 0)	return m_joints[m_parts[0].m_parentJoint].angle;
			else								return m_parts[0].getShape().getAngle();
		}
		else									return m_shape.getAngle();
	}
	virtual void rotate(float angle) override;
	virtual void rotate(float angle, glm::vec2 pos) override;
	virtual void scale(glm::vec2 scale) override;
	virtual void incrPos(glm::vec2 offset) override;
	virtual void setPos(glm::vec2 pos) override;
	virtual void setScale(glm::vec2 scale) override;
	virtual void setCenter(glm::vec2 center) override;
	virtual void setAngle(float angle) override;
#pragma endregion VIRTUAL PUBLIC METHODS -------------------------------------------------------------------------------

#pragma region
	bool isForcesNotNull() const { return m_forces.x != 0.0f && m_forces.y != 0.0f; }
	float getMass() const { return m_mass; }
	float getASpeed() const { return m_angularSpeed; }
	float getOrientation() const{ return m_orientation; }
	float getAngularVelocity() const { return m_angularVelocity; }
	int getParentPart(unsigned int partIndex) const;
	int getParentJoint(unsigned int partIndex) const;
	const std::vector<Part>& getParts() const { return m_parts; }
	const std::vector<Joint2D>& getJoints() const { return m_joints; }
	const Shape2D& getShape() const { return m_shape; }
	const Shape2D& getRealShape() const {
		if (!m_parts.empty())
			return m_parts[0].getShape();
		else
			return m_shape;
	}
	
	float getWidth() const {
		return (!m_parts.empty()) ? 
			m_parts[0].getShape().getWidth() :
			m_shape.getWidth();
	}
	float getHeight() const {
		return (!m_parts.empty()) ?
			m_parts[0].getShape().getHeight() :
			m_shape.getHeight();
	}
	glm::vec2 getDims() const {
		return (!m_parts.empty()) ?
			m_parts[0].getShape().getDims() :
			m_shape.getDims();
	}
	glm::vec2 getImpulse() const{ return m_forces; }
	float getDepth() const;
	glm::vec2 getCenter() const;

	std::vector<Joint2D>& getJoints() { return m_joints; }
	Shape2D& getShape() { return m_shape; }
	Part& getPart(uint16_t index) { return m_parts[index]; }
	Joint2D& getJoint(uint16_t index) { return m_joints[index]; }
	std::vector<Part>& getParts()  { return m_parts; }
	MaterialType getMaterial() const { return m_material; }
	BodyType getType() const { return m_type; }
	glm::vec2 getMidleFacePoint() const;
#pragma endregion GETTER METHODS  ---------------------------------------------------------------------------------------

#pragma region
	/*!
	 * \brief Set the current orientation (in radian).
	 */
	void setOrientation(float orientation) { m_orientation = orientation; }
	void setMaterial(MaterialType material) { m_material = material; }
#pragma endregion SETTER METHODS  ---------------------------------------------------------------------------------------

protected:

#pragma region
	/*!
	* \brief Update the surronding shape circle of the body.
	*/
	void updateCircle();
	/*!
	* \brief Update the surronding shape box of the body.
	*/
	void updateBox();
	/*!
	* \brief Apply a move of the given part.
	*/
	void move(unsigned int partIndex, glm::vec2 offset);
	/*!
	* \brief Apply a move of the given joint.
	*/
	void move(unsigned int partIndex, const Move2D& m, glm::vec2 rotPoint);
	void rotate(unsigned int partIndex, float angle);
	void rotate(unsigned int partIndex, float angle, glm::vec2 pos);
	void scale(unsigned int partIndex, glm::vec2 scale);
	void scale(unsigned int partIndex, glm::vec2 scale, glm::vec2 pos);
	void setPos(unsigned int partIndex, glm::vec2 pos);
	void setScale(unsigned int partIndex, glm::vec2 scale);
	void setAngle(unsigned int partIndex, float angle);
	void initAngle(unsigned int partIndex, float angle);
	void initAngle(unsigned int partIndex, float angle, glm::vec2 pos);
	void initScale(unsigned int partIndex, glm::vec2 scale);
	void initScale(unsigned int partIndex, glm::vec2 scale, glm::vec2 pos);
	void initPos(unsigned int partIndex, glm::vec2 pos);

#pragma endregion PROTECTED CLASS METHODS  ------------------------------------------------------------------------------

#pragma region
	float m_mass = 1.0f;				/* Total mass (constant) */
	float m_invMass = 0.0f;
	float m_torque = 0.0f;
	float m_angularSpeed = 0.1f;		/* Angular speed */
	float m_angularVelocity = 0.0f;		/* Angular velocity */
	float m_orientation = 0.0f;			/* Body orientation */
	float m_initAngle = 0.0f;			/* Initial angle */
	bool m_needUpdate = false;			/* Inform if the body has moved since the last time */

	MaterialType m_material = MaterialType::ORGANIC;
	BodyType m_type = BodyType::BODY_EMPTY;

	glm::vec2 m_forces = glm::vec2(0.0f);				/* Impulse */
	glm::vec2 m_initPos = glm::vec2(0.0f);				/* Initial position */
	glm::vec2 m_initScale = glm::vec2(1.0f);			/* Initial scale */
	
	std::vector<Part> m_parts;			/* Body parts */
	std::vector<Joint2D> m_joints;		/* Body joints */
	Shape2D m_shape;					/* Collision Bounding shape */
#pragma endregion CLASS VARIABLES  --------------------------------------------------------------------------------------

};


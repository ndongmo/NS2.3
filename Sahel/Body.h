/*!
* \file Body.h
* \brief Physical body.
* \author Ndongmo Silatsa F.
* \date 05-04-2017
* \version 2.0
*/

#pragma once

#include "BodyPart.h"
#include "IFileManager.h"
#include "MoveChange.h"

class ObjectLoader;
class ObjectSaver;
class BodyEditor;

#define SPEED_FACTOR 0.006f

class Body : public ITransform, public IDraw, public IFileManager
{
	friend ObjectLoader;
	friend ObjectSaver;
	friend BodyEditor;

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
	void setPart(unsigned int index, BodyPart& part);
	/*!
	* \brief Add an angular impulse with the given angle value.
	*/
	void addAngularImpulse(float angle);
	/*!
	* \brief add an impulse with the given velocity values.
	*/
	void addImpulse(float x, float y);
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
	void incrScale(unsigned int partIndex, float x, float y);
	/*!
	 * \brief Increment the current body scale.
	 */
	void incrScale(float x, float y);
	/*!
	 * \brief Apply the given move to the body.
	 */
	void move(const Move2D& m);
	void incrPos(unsigned int partIndex, float x, float y);

#pragma endregion PUBLIC CLASS METHODS ---------------------------------------------------------------------------------

#pragma region
	virtual void draw(NS2::SpriteBatch& spriteBatch) const override;
	virtual void drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const override;
	virtual float getPosX() const override;
	virtual float getPosY() const override;
	virtual float getScaleX() const override {
		if (!m_parts.empty())	return m_parts[0].getConstShape().getScaleX();
		else					return m_shape.getScaleX();
	}
	virtual float getScaleY() const override {
		if (!m_parts.empty())	return m_parts[0].getConstShape().getScaleY();
		else					return m_shape.getScaleY();
	}
	virtual float getAngle() const override {
		if (!m_parts.empty()) {
			if (m_parts[0].m_parentJoint >= 0)	return m_joints[m_parts[0].m_parentJoint].angle * RADTODEG;
			else								return m_parts[0].getConstShape().getAngle() * RADTODEG;
		}
		else									return m_shape.getAngle() * RADTODEG;
	}
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
	virtual void setScale(float x, float y);
#pragma endregion VIRTUAL PUBLIC METHODS -------------------------------------------------------------------------------

#pragma region
	bool isForcesNotNull() const { return m_forces.x != 0.0f && m_forces.y != 0.0f; }
	float getMass() const { return m_mass; }
	float getASpeed() const { return m_angularSpeed; }
	float getDensity() const { return m_density; }
	float getFriction() const { return m_friction; }
	float getRestitution() const { return m_restitution; }
	float getAngleRad() const {
		if (!m_parts.empty()) {
			if (m_parts[0].m_parentJoint >= 0)
				return m_joints[m_parts[0].m_parentJoint].angle;
			else
				return m_parts[0].getConstShape().getAngle();
		}
		else {
			return m_shape.getAngle();
		}
	}
	float getOrientation() const{ return m_orientation; }
	float getAngularVelocity() const { return m_angularVelocity; }
	int getParentPart(unsigned int partIndex) const;
	int getParentJoint(unsigned int partIndex) const;
	const std::vector<BodyPart>& getConstParts() const { return m_parts; }
	const std::vector<Joint2D>& getConstJoints() const { return m_joints; }
	const Shape2D& getConstShape() const { return m_shape; }
	const Shape2D& getConstRealShape() const {
		if (!m_parts.empty())
			return m_parts[0].getConstShape();
		else
			return m_shape;
	}
	Shape2D& getRealShape() {
		if (!m_parts.empty())
			return m_parts[0].getShape();
		else
			return m_shape;
	}
	
	float getWidth() const {
		return (!m_parts.empty()) ? 
			m_parts[0].getConstShape().getWidth() :
			m_shape.getWidth();
	}
	float getHeight() const {
		return (!m_parts.empty()) ?
			m_parts[0].getConstShape().getHeight() :
			m_shape.getHeight();
	}
	const glm::vec2& getConstDims() const {
		return (!m_parts.empty()) ?
			m_parts[0].getConstShape().getConstDims() :
			m_shape.getConstDims();
	}
	const glm::vec2& getConstImpulse() const{ return m_forces; }
	glm::vec2 getImpulse() const{ return m_forces; }
	float getDepth() const;
	const glm::vec2& getConstCenter() const;
	glm::vec2 getPos() const;

	std::vector<Joint2D>& getJoints() { return m_joints; }
	Shape2D& getShape() { return m_shape; }
	BodyPart& getPart(uint16_t index) { return m_parts[index]; }
	Joint2D& getJoint(uint16_t index) { return m_joints[index]; }
	std::vector<BodyPart>& getParts()  { return m_parts; }
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
	* \brief Update the surronding shape polygon of the body.
	*/
	void updatePolygon();
	/*!
	* \brief Apply a move of the given part around x axis.
	*/
	void moveX(unsigned int partIndex, float x);
	/*!
	* \brief Apply a move of the given part around y axis.
	*/
	void moveY(unsigned int partIndex, float y);
	/*!
	* \brief Apply a move of the given joint.
	*/
	void move(unsigned int partIndex, const Move2D& m, const glm::vec2& rotPoint);
	void rotate(unsigned int partIndex, float angle);
	void rotate(unsigned int partIndex, float angle, const glm::vec2& pos);
	void scaleX(unsigned int partIndex, float x);
	void scaleY(unsigned int partIndex, float y);
	void scale(unsigned int partIndex, float x, float y);
	void scaleX(unsigned int partIndex, float x, float posX);
	void scaleY(unsigned int partIndex, float y, float posY);
	void incrPosX(unsigned int partIndex, float x);
	void incrPosY(unsigned int partIndex, float y);
	void setPos(unsigned int partIndex, float x, float y);
	/*!
	* \brief The following methods initialize the given part according to the given parameter
	*	On the contrary of the other initializer methods, they do not change joint position!!!!
	*/
	void initAngle(unsigned int partIndex, float angle);
	void initAngle(unsigned int partIndex, float angle, const glm::vec2& pos);
	void initScaleX(unsigned int partIndex, float x);
	void initScaleY(unsigned int partIndex, float y);
	void initScale(unsigned int partIndex, float x, float y);
	void initScaleX(unsigned int partIndex, float x, float posX);
	void initScaleY(unsigned int partIndex, float y, float posY);
	void initPos(unsigned int partIndex, float x, float y);

#pragma endregion PROTECTED CLASS METHODS  ------------------------------------------------------------------------------

#pragma region
	float m_density = 0.0f;				/* Collision density */
	float m_friction = 0.0f;			/* Collision friction */
	float m_restitution = 0.0f;			/* Collision restitution */
	float m_mass = 1.0f;				/* Total mass (constant) */
	float m_invMass = 0.0f;
	float m_torque = 0.0f;
	float m_angularSpeed = 0.1f;		/* Angular speed */
	float m_angularVelocity = 0.0f;		/* Angular velocity */
	float m_orientation = 0.0f;			/* Body orientation */
	float m_initAngle = 0.0f;			/* Initial angle */
	bool m_needUpdate = false;			/* Inform if the body has moved since the last time */

	MaterialType m_material = MaterialType::NONE;
	BodyType m_type = BodyType::BODY_EMPTY;

	glm::vec2 m_forces = glm::vec2(0.0f);				/* Impulse */
	glm::vec2 m_initPos = glm::vec2(0.0f);				/* Initial position */
	glm::vec2 m_initScale = glm::vec2(1.0f);			/* Initial scale */
	
	std::vector<BodyPart> m_parts;		/* Body parts */
	std::vector<Joint2D> m_joints;		/* Body joints */
	Shape2D m_shape;					/* Collision Bounding shape */
#pragma endregion CLASS VARIABLES  --------------------------------------------------------------------------------------

};


/*!
* \file ITransform.h
* \brief Transformation interface.
* It contents all the virtual transformation methods and
* should be used for objects wich need to proceed a transformation.
* \author Ndongmo Silatsa Fabrice
* \date 16-05-2017
* \version 2.0
*/

#pragma once

#include "Move2D.h"
#include "MoveChange.h"

class ITransform
{
public:

	/*!
	* \brief get angle.
	*/
	virtual float getAngle() const = 0;
	/*!
	* \brief get position.
	*/
	virtual glm::vec2 getPos() const = 0;
	/*!
	 * \brief get scale.
	 * \return scale
	 */
	virtual glm::vec2 getScale() const = 0;
	/*!
	* \brief Apply a the given move.
	* \param m : move to create
	*/
	virtual void apply(const MoveChange& m) {}
	/*!
	* \brief Apply a 2D move.
	* \param m : move to create
	*/
	virtual void move(const Move2D& m, glm::vec2 pos) {}
	/*!
	* \brief Rotate around his rotation point.
	* \param angle : rotation angle in radian
	*/
	virtual void rotate(float angle) = 0;
	/*!
	* \brief Rotate around a given rotation point.
	* \param angle : rotation angle in radian
	* \param pos : rotation point
	*/
	virtual void rotate(float angle, glm::vec2 pos) {}
	/*!
	* \brief Scale along the x and y axes.
	* \param x : x factor
	* \param y : y factor
	*/
	virtual void scale(glm::vec2 scale) = 0;
	/*!
	* \brief Scale along the x and y axes.
	* \param x : x factor
	* \param y : y factor
	* \param posX : x coordinate of scale axis
	* \param posY : y coordinate of scale axis
	*/
	virtual void scale(glm::vec2 scale, glm::vec2 pos) {};
	/*!
	* \brief Increment or decrement the x and y coordinates.
	* \param x : x value
	* \param y : y value
	*/
	virtual void incrPos(glm::vec2 pos) = 0;
	/*!
	* \brief Increment or decrement the scale on 2D dimensions.
	* \param x : x value
	* \param y : y value
	*/
	virtual void incrScale(glm::vec2 scale) {}
	/*!
	* \brief Set the x and y coordinates.
	*/
	virtual void setPos(glm::vec2 pos) = 0;
	/*!
	* \brief Set the scale.
	*/
	virtual void setScale(glm::vec2 scale) {}
	/*!
	* \brief Move the center position to the given position.
	*/
	virtual void setCenter(glm::vec2 center) = 0;
	/*!
	* \brief Set angle.
	* \param angle :
	*/
	virtual void setAngle(float angle) = 0;
	/*!
	* \brief Set angle around a given rotation point.
	* \param angle : rotation angle
	* \param pos : rotation point
	*/
	virtual void setAngle(float angle, glm::vec2 pos) {}
};


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
	* \brief get x position.
	*/
	virtual float getPosX() const = 0;
	/*!
	* \brief get y position.
	*/
	virtual float getPosY() const = 0;
	/*!
	* \brief get angle.
	*/
	virtual float getAngle() const = 0;
	/*!
	 * \brief get scale on x axis.
	 * \return scale on x axis
	 */
	virtual float getScaleX() const = 0;
	/*!
	 * \brief get scale on y axis.
	 * \return scale on y axis
	 */
	virtual float getScaleY() const = 0;
	/*!
	* \brief Apply a the given move.
	* \param m : move to create
	*/
	virtual void apply(const MoveChange& m) {}
	/*!
	* \brief Apply a 2D move.
	* \param m : move to create
	*/
	virtual void move(const Move2D& m, const glm::vec2& pos) {}
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
	virtual void rotate(float angle, const glm::vec2& pos) {}
	/*!
	* \brief Scale along the x axis.
	* \param x : x factor
	*/
	virtual void scaleX(float x) = 0;
	/*!
	* \brief Scale along the y axis.
	* \param y : y factor
	*/
	virtual void scaleY(float y) = 0;
	/*!
	* \brief Scale along the x and y axes.
	* \param x : x factor
	* \param y : y factor
	*/
	virtual void scale(float x, float y) = 0;
	/*!
	* \brief Scale along the x axis.
	* \param x : x factor
	* \param posX : x coordinate of scale axis
	*/
	virtual void scaleX(float x, float posX) {};
	/*!
	* \brief Scale along the y axis.
	* \param y : y factor
	* \param posY : y coordinate of scale axis
	*/
	virtual void scaleY(float y, float posY) {};
	/*!
	* \brief Scale along the x and y axes.
	* \param x : x factor
	* \param y : y factor
	* \param posX : x coordinate of scale axis
	* \param posY : y coordinate of scale axis
	*/
	virtual void scale(float x, float y, float posX, float posY) {};
	/*!
	* \brief Increment or decrement the x coordinate.
	* \param x : x value
	*/
	virtual void incrPosX(float x) = 0;
	/*!
	* \brief Increment or decrement the y coordinate.
	* \param y : y value
	*/
	virtual void incrPosY(float y) = 0;
	/*!
	* \brief Increment or decrement the x and y coordinates.
	* \param x : x value
	* \param y : y value
	*/
	virtual void incrPos(float x, float y) = 0;
	/*!
	* \brief Increment or decrement the scale on x.
	* \param x : x value
	*/
	virtual void incrScaleX(float x) {}
	/*!
	* \brief Increment or decrement the scale on y.
	* \param y : y value
	*/
	virtual void incrScaleY(float y) {}
	/*!
	* \brief Increment or decrement the scale on 2D dimensions.
	* \param x : x value
	* \param y : y value
	*/
	virtual void incrScale(float x, float y) {}
	/*!
	* \brief Set the x and y coordinates.
	* \param x : x value
	* \param y : y value
	*/
	virtual void setPos(float x, float y) = 0;
	/*!
	* \brief Move the center position to the given position.
	* \param x : x value
	* \param y : y value
	*/
	virtual void setCenter(float x, float y) = 0;
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
	virtual void setAngle(float angle, const glm::vec2& pos) {}
};


/*!
* \file Joint2D.h
* \brief 2D Joint structure.
* \author Ndongmo Silatsa Fabrice
* \date 16-05-2017
* \version 2.0
*/

#pragma once

#include "ITransform.h"
#include "MathUtils.h"

class Joint2D
{
public:
	/*!
	 * \brief Updates the joint rotation point with the given move.
	 */
	void update(const Move2D& m, const glm::vec2& pos);
	/*!
	 * \brief Updates the joint angle.
	 * \param angle : in radiant
	 */
	void rotate(float angle);

	/*!
	* \brief Correct the given value considering the upper and the lower value.
	*/
	void correctValue(Move2D& m);

	unsigned int index = 0;					/* Index in the body joint's vector */
	unsigned int parent = 0;				/* Parent body part */
	unsigned int son = 0;					/* Son body part */
	float angle = 0.0f;						/* Current angle. Stores the previous rotation undergone by the joint */
	glm::vec2 point{ 0.0f, 0.0f };			/* Rotation point in earth coordinates */
	Move2D lower;							/* Could be lower angle for Angle joint or lower x/y for Axis joint */
	Move2D upper;							/* Could be upper angle for Angle joint or upper x/y for Axis joint */
	Move2D value;							/* Current joint value */
};


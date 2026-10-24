/*!
* \file LinkedBody.h
* \brief 2D Joint structure.
* \author Ndongmo Silatsa Fabrice
* \date 16-05-2017
* \version 2.0
*/

#pragma once

#include "ITransform.h"

#include <vector>

class ILinkedBody
{
public:
	/*!
	* \brief Attachs a given movable body to the current body.
	* \param link : the body to attach
	*/
	void lattach(ITransform* link);
	/*!
	* \brief Detachs a given movable body attached to the current body.
	* \param link : the body to detach
	*/
	void ldetach(ITransform* link);
	/*!
	* \brief Detachs all movable bodies attached to the current body.
	* \param link : the body to detach
	*/
	void ldetachAll();

	void lrotate(float angle, const glm::vec2& pos);
	void lincrPosX(float x);
	void lincrPosY(float y);
	void lincrPos(float x, float y);
	void lsetPos(float x, float y);
	void lsetCenter(float x, float y);
	void lsetAngle(float angle, const glm::vec2& pos);

protected:
	std::vector<ITransform*> m_links;
};


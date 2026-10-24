/*!
* \file IDraw.h
* \brief Drawing interface. It is used by objects wich need to draw.
* \author Ndongmo Silatsa Fabrice
* \date 16-05-2017
* \version 2.0
*/

#pragma once

#include <SpriteBatch.h>
#include <DebugRenderer.h>

class IDraw
{
public:
	virtual ~IDraw() = default;

	/*!
	* \brief Draw a specific object.
	* \param spriteBatch : drawing batch
	*/
	virtual void draw(NS2::SpriteBatch& spriteBatch) const {}
	/*!
	* \brief Draw a specific object in debug mode.
	* \param debugRenderer : renderer in debug mode
	*/
	virtual void drawDebug(NS2::DebugRenderer& debugRenderer) const {}
	/*!
	* \brief Draw a specific object in debug mode.
	* \param debugRenderer : renderer in debug mode
	* \param color : rendering color
	*/
	virtual void drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const {}
};


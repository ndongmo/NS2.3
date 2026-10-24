/*!
* \file MoveChange.h
* \brief A single change animation.
* \author Ndongmo Silatsa F.
* \date 22-01-2022
* \version 1.0
*/

#pragma once

#include <glm\glm.hpp>

struct MoveChange
{
	/*!
	* \brief MoveChange empty constructor.
	*/
	MoveChange() {}
	/*!
	* \brief MoveChange constructor.
	*/
	MoveChange(float x, float y, float angle, float scaleX, float scaleY, 
		float r, float g, float b, float alpha) :
		pos(x, y), angle(angle), scaleX(scaleX), scaleY(scaleY),
		r(r), g(g), b(b), alpha(alpha)
	{}

	/*!
	 * \brief Resets move values.
	 */
	void reset() { 
		pos.x = pos.y = angle = scaleX = scaleY = 0.0f;
		r = g = b = 0;
		alpha = 0;
	}
	/*!
	 * \brief Check if the current Move is null.
	 */
	bool isNull() const
	{
		return pos.x == 0.0f && pos.y == 0.0f && angle == 0.0f &&
			scaleX == 0.0f && scaleY == 0.0f &&
			r == 0 && g == 0 && b == 0 && alpha == 0;
	}
	/*!
	 * \brief Negate all values.
	 */
	void negate()
	{
		pos.x = -pos.x;
		pos.y = -pos.y;
		angle = -angle;
		scaleX = -scaleX;
		scaleY = -scaleY;
		r = -r;
		g = -g;
		b = -b;
		alpha = -alpha;
	}

	bool operator == (const MoveChange& m)
	{
		return pos.x == m.pos.x && pos.y == m.pos.y && angle == m.angle && 
			scaleX == m.scaleX && scaleY == m.scaleY && r == m.r && 
			g == m.g && b == m.b && alpha == m.alpha;
	}
	bool operator != (const MoveChange& m)
	{
		return pos.x != m.pos.x || pos.y != m.pos.y || angle != m.angle ||
			scaleX != m.scaleX || scaleY != m.scaleY || r != m.r ||
			g != m.g || b != m.b || alpha != m.alpha;
	}

	void operator /= (const MoveChange& m)
	{
		pos.x /= m.pos.x; pos.y /= m.pos.y; angle /= m.angle;
		scaleX /= m.scaleX; scaleY /= m.scaleY;
		r /= m.r; g /= m.g; b /= m.b; alpha /= m.alpha;
	}
	void operator *= (const MoveChange& m)
	{
		pos.x *= m.pos.x; pos.y *= m.pos.y; angle *= m.angle;
		scaleX *= m.scaleX; scaleY *= m.scaleY;
		r *= m.r; g *= m.g; b *= m.b; alpha *= m.alpha;
	}
	void operator -= (const MoveChange& m)
	{
		pos.x -= m.pos.x; pos.y -= m.pos.y; angle -= m.angle;
		scaleX -= m.scaleX; scaleY -= m.scaleY;
		r -= m.r; g -= m.g; b -= m.b; alpha -= m.alpha;
	}
	void operator += (const MoveChange& m)
	{
		pos.x += m.pos.x; pos.y += m.pos.y; angle += m.angle;
		scaleX += m.scaleX; scaleY += m.scaleY;
		r += m.r; g += m.g; b += m.b; alpha += m.alpha;
	}

	MoveChange operator + (float i) const
	{
		return MoveChange(pos.x + i, pos.y + i, angle + i,
			scaleX + i, scaleY + i, r + i, g + i, b + i, alpha + i);
	}
	MoveChange operator - (float i) const
	{
		return MoveChange(pos.x - i, pos.y - i, angle - i,
			scaleX - i, scaleY - i, r - i, g - i, b - i, alpha - i);
	}
	MoveChange operator * (float i) const
	{
		return MoveChange(pos.x * i, pos.y * i, angle * i,
			scaleX * i, scaleY * i, r * i, g * i, b * i, alpha * i);
	}
	MoveChange operator / (float i) const
	{
		return MoveChange(pos.x / i, pos.y / i, angle / i,
			scaleX / i, scaleY / i, r / i, g / i, b / i, alpha / i);
	}
	MoveChange operator + (const MoveChange& m) const
	{
		return MoveChange(pos.x + m.pos.x, pos.y + m.pos.y, angle + m.angle, scaleX + m.scaleX,
			scaleY + m.scaleY, r + m.r, g + m.g, b + m.b, alpha + m.alpha);
	}
	MoveChange operator - (const MoveChange& m) const
	{
		return MoveChange(pos.x - m.pos.x, pos.y - m.pos.y, angle - m.angle, scaleX - m.scaleX,
			scaleY - m.scaleY, r - m.r, g - m.g, b - m.b, alpha - m.alpha);
	}
	MoveChange operator - () const // unary minus
	{
		return MoveChange(-pos.x, -pos.y, -angle, -scaleX, -scaleY, -r, -g, -b, -alpha);
	}
	MoveChange operator * (const MoveChange& m) const
	{
		return MoveChange(pos.x * m.pos.x, pos.y * m.pos.y, angle * m.angle, scaleX * m.scaleX,
			scaleY * m.scaleY, r * m.r, g * m.g, b * m.b, alpha * m.alpha);
	}
	MoveChange operator / (const MoveChange& m) const
	{
		return MoveChange(pos.x / m.pos.x, pos.y / m.pos.y, angle / m.angle, scaleX / m.scaleX,
			scaleY / m.scaleY, r / m.r, g / m.g, b / m.b, alpha / m.alpha);
	}

	glm::vec2 pos { 0.0f, 0.0f };	/* Coordinate x and y */
	float angle = 0.0f;				/* Angle */
	float scaleX = 0.0f;			/* Scale x */
	float scaleY = 0.0f;			/* Scale y */
	float r = 0;					/* red color value */
	float g = 0;					/* green color value */
	float b = 0;					/* blue color value */
	float alpha = 0;				/* alpha color value */
};
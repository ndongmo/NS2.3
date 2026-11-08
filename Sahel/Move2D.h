/*!
* \file Move2D.h
* \brief Move 2D structure.
* \author Ndongmo Silatsa Fabrice
* \date 25-03-2017
* \version 2.0
*/

#pragma once

#include <glm/glm.hpp>

/*!
* \brief 2D move structure.
*/
struct Move2D
{
	/*!
	* \brief Move2D constructor.
	*/
	Move2D() : pos(0.0f, 0.0f), a(0.0f){}
	/*!
	* \brief Move2D constructor.
	* \param x :  x axis coordinate
	* \param y :  y axis coordinate
	* \param a :  angle
	*/
	Move2D(float x, float y, float a) : pos(x, y), a(a) {}
	/*!
	* \brief Resets move values.
	*/
	void reset(){ pos = glm::vec2(0.0f, 0.0f); a = 0.0f; }
	/*!
	* \brief Sets move values.
	*/
	void setValues(float x, float y, float a)
	{
		this->pos = glm::vec2(x, y);
		this->a = a;
	}
	/*!
	* \brief Inverse y coordinate and angle.
	*/
	void inverseYaxis() 
	{
		pos.y *= -1;
		a *= -1;
	}
	void negate()
	{
		pos = -pos;
		a = -a;
	}

	bool isNull()
	{
		return pos == glm::vec2(0.0f, 0.0f) && a == 0.0f;
	}

	void operator /= (float i)
	{
		pos /= i; a /= i;
	}
	void operator *= (float i)
	{
		pos *= i; a *= i;
	}

	void operator -= (float i)
	{
		pos -= i; a -= i;
	}

	void operator += (float i)
	{
		pos += i; a += i;
	}

	void operator /= (const Move2D& m)
	{
		pos /= m.pos; a /= m.a;
	}
	void operator *= (const Move2D& m)
	{
		pos *= m.pos; a *= m.a;
	}
	void operator -= (const Move2D& m)
	{
		pos -= m.pos; a -= m.a;
	}
	void operator += (const Move2D& m)
	{
		pos += m.pos; a += m.a;
	}

	Move2D operator + (float i) const
	{
		return Move2D(pos.x + i, pos.y + i, a + i);
	}
	Move2D operator - (float i) const
	{
		return Move2D(pos.x - i, pos.y - i, a - i);
	}
	Move2D operator * (float i) const
	{
		return Move2D(pos.x * i, pos.y * i, a * i);
	}
	Move2D operator / (float i) const
	{
		return Move2D(pos.x / i, pos.y / i, a / i);
	}
	Move2D operator + (const Move2D& m) const
	{
		return Move2D(pos.x + m.pos.x, pos.y + m.pos.y, a + m.a);
	}
	Move2D operator - (const Move2D& m) const
	{
		return Move2D(pos.x - m.pos.x, pos.y - m.pos.y, a - m.a);
	}
	Move2D operator - () const // unary minus
	{
		return Move2D(-pos.x, -pos.y, -a);
	}
	Move2D operator * (const Move2D& m) const
	{
		return Move2D(pos.x * m.pos.x, pos.y * m.pos.y, a * m.a);
	}
	Move2D operator / (const Move2D& m) const
	{
		return Move2D(pos.x / m.pos.x, pos.y / m.pos.y, a / m.a);
	}

	bool operator == (const Move2D& m) const
	{
		return pos == m.pos && a == m.a;
	}
	bool operator < (const Move2D& m) const
	{
		return pos.x < m.pos.x || pos.y < m.pos.y || a < m.a;
	}
	bool operator >(const Move2D& m) const
	{
		return pos.x > m.pos.x || pos.y > m.pos.y || a > m.a;
	}
	bool operator <= (const Move2D& m) const
	{
		return pos.x <= m.pos.x && pos.y <= m.pos.y && a <= m.a;
	}
	bool operator >= (const Move2D& m) const
	{
		return pos.x >= m.pos.x && pos.y >= m.pos.y && a >= m.a;
	}
	bool operator != (const Move2D& m) const
	{
		return pos != m.pos || a != m.a;
	}

	glm::vec2 pos;	// position
	float a;		// angle
};
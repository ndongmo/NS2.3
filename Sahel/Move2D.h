/*!
* \file Move2D.h
* \brief Move 2D structure.
* \author Ndongmo Silatsa Fabrice
* \date 25-03-2017
* \version 2.0
*/

#pragma once

/*!
* \brief 2D move structure.
*/
struct Move2D
{
	/*!
	* \brief Move2D constructor.
	*/
	Move2D() : x(0.0f), y(0.0f), a(0.0f){}
	/*!
	* \brief Move2D constructor.
	* \param x :  x axis coordinate
	* \param y :  y axis coordinate
	* \param a :  angle
	*/
	Move2D(float x, float y, float a) : x(x), y(y), a(a) {}
	/*!
	* \brief Resets move values.
	*/
	void reset(){ x = y = a = 0.0f; }
	/*!
	* \brief Sets move values.
	*/
	void setValues(float x, float y, float a)
	{
		this->x = x;
		this->y = y;
		this->a = a;
	}
	/*!
	* \brief Inverse y coordinate and angle.
	*/
	void inverseYaxis() 
	{
		y *= -1;
		a *= -1;
	}
	void negate()
	{
		x = -x;
		y = -y;
		a = -a;
	}

	bool isNull()
	{
		return x == 0.0f && y == 0.0f && a == 0.0f;
	}

	void operator /= (float i)
	{
		x /= i; y /= i; a /= i;
	}
	void operator *= (float i)
	{
		x *= i; y *= i; a *= i;
	}

	void operator -= (float i)
	{
		x -= i; y -= i; a -= i;
	}

	void operator += (float i)
	{
		x += i; y += i; a += i;
	}

	void operator /= (const Move2D& m)
	{
		x /= m.x; y /= m.y; a /= m.a;
	}
	void operator *= (const Move2D& m)
	{
		x *= m.x; y *= m.y; a *= m.a;
	}
	void operator -= (const Move2D& m)
	{
		x -= m.x; y -= m.y; a -= m.a;
	}
	void operator += (const Move2D& m)
	{
		x += m.x; y += m.y; a += m.a;
	}

	Move2D operator + (float i) const
	{
		return Move2D(x + i, y + i, a + i);
	}
	Move2D operator - (float i) const
	{
		return Move2D(x - i, y - i, a - i);
	}
	Move2D operator * (float i) const
	{
		return Move2D(x * i, y * i, a * i);
	}
	Move2D operator / (float i) const
	{
		return Move2D(x / i, y / i, a / i);
	}
	Move2D operator + (const Move2D& m) const
	{
		return Move2D(x + m.x, y + m.y, a + m.a);
	}
	Move2D operator - (const Move2D& m) const
	{
		return Move2D(x - m.x, y - m.y, a - m.a);
	}
	Move2D operator - () const // unary minus
	{
		return Move2D(-x, -y, -a);
	}
	Move2D operator * (const Move2D& m) const
	{
		return Move2D(x * m.x, y * m.y, a * m.a);
	}
	Move2D operator / (const Move2D& m) const
	{
		return Move2D(x / m.x, y / m.y, a / m.a);
	}

	bool operator == (const Move2D& m) const
	{
		return x == m.x && y == m.y && a == m.a;
	}
	bool operator < (const Move2D& m) const
	{
		return x < m.x || y < m.y || a < m.a;
	}
	bool operator >(const Move2D& m) const
	{
		return x > m.x || y > m.y || a > m.a;
	}
	bool operator <= (const Move2D& m) const
	{
		return x <= m.x && y <= m.y && a <= m.a;
	}
	bool operator >= (const Move2D& m) const
	{
		return x >= m.x && y >= m.y && a >= m.a;
	}
	bool operator != (const Move2D& m) const
	{
		return x != m.x || y != m.y || a != m.a;
	}

	float x;	// x axis coordinate
	float y;	// y axis coordinate
	float a;	// angle
};
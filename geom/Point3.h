#pragma once

#include "Vector3.h"

class Point3 : public Vector3
{
public:
	inline Point3() : Vector3(0.0, 0.0, 0.0) {}

	inline Point3(double x, double y, double z) 
		: Vector3(x, y, z) {}	

	inline Point3(const Vector3 &v) 
		: Vector3(v.x, v.y, v.z) {}

	Point3(const Point3 *p)
		: Vector3(p->x, p->y, p->z) {}

	// Copy Constructor
	Point3(const Point3 &p) 
		: Vector3(p.x, p.y, p.z) {}

// ---------------------------------------------------------------------------------------------------- //

	inline bool operator==(const Point3 &p) const
	{
		return x == p.x && y == p.y && z == p.z;
	}

	inline bool operator!=(const Point3 &p) const
	{
		return x != p.x || y != p.y || z != p.z;
	}

	inline Point3 operator+(const Vector3 v) const
	{
		return Point3(x+v.x, y+v.y, z+v.z);
	}

	inline Point3 & operator+=(const Vector3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Point3 operator-(const Vector3 &v) const
	{
		return Point3(x-v.x, y-v.y, z-v.z);
	}

	inline Point3 & operator-=(const Vector3 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Point3 operator-() const
	{
		return Point3(-x, -y, -z);
	}

};

// ---------------------------------------------------------------------------------------------------- //

// Writes Point3 p to output stream os.
inline std::ostream & operator<<(std::ostream &os, const Point3 &p)
{
	os << p.x << " " << p.y << " " << p.z;
	return os;
}
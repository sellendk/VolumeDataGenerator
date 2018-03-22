#ifndef VEC3_HPP
#define VEC3_HPP

template <typename T>
class Vec3
{
public:
	T x = 0.0;
	T y = 0.0;
	T z = 0.0;

	Vec3() {}

	Vec3(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3(T value)
	{
		this->x = value;
		this->y = value;
		this->z = value;
	}

	Vec3(const Vec3 &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	inline double length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	inline T dist(const Vec3 other) const
	{
		return sqrt(pow((x - other.x), 2.0)
			+ pow((y - other.y), 2.0) + pow((z - other.z), 2.0));
	}

	void normalize()
	{
		T length = this->length();

		if (length != 0)
		{
			this->x /= length;
			this->y /= length;
			this->z /= length;
		}
	}

	Vec3 normalise()
	{
		return normalize();
	}

	Vec3 abs()
	{
		return Vec3<T>(fabs(this->x), fabs(this->y), fabs(this->z));
	}

	inline void Scale(T s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
	}

	inline void Scale(Vec3 s)
	{
		this->x *= s.x;
		this->y *= s.y;
		this->z *= s.z;
	}

	Vec3 operator+ (const Vec3 &right)
	{
		Vec3 v(*this);
		v.x = this->x + right.x;
		v.y = this->y + right.y;
		v.z = this->z + right.z;
		return v;
	}

	Vec3 &operator+= (const Vec3 &right)
	{
		this->x += right.x;
		this->y += right.y;
		this->z += right.z;
		return *this;
	}

	Vec3 operator- (const Vec3 &right) const
	{
		Vec3 v(*this);
		v.x = this->x - right.x;
		v.y = this->y - right.y;
		v.z = this->z - right.z;
		return v;
	}

	Vec3 &operator-= (const Vec3 &right)
	{
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
		return *this;
	}

	Vec3 operator- () const
	{
		Vec3 v(*this);
		v.Scale(-1);
		return v;
	}

	Vec3 operator* (const Vec3 &right) const
	{
		Vec3 s(*this);
		s.Scale(right);
		return s;
	}

	Vec3 operator* (T &scale) const
	{
		Vec3 s(*this);
		s.Scale(scale);
		return s;
	}

	Vec3 &operator*= (T scale)
	{
		this->x *= scale;
		this->y *= scale;
		this->z *= scale;
		return *this;
	}

	Vec3 &operator*= (const Vec3 right)
	{
		this->x *= right.x;
		this->y *= right.y;
		this->z *= right.z;
		return *this;
	}

	Vec3 operator/ (const Vec3 &right)
	{
		Vec3 v(*this);
		v.x /= right.x;
		v.y /= right.y;
		v.z /= right.z;
		return v;
	}

	Vec3 operator/ (T &div)
	{
		Vec3 s(*this);
		s.Scale(1.0 / div);
		return s;
	}

	Vec3 &operator/= (T div)
	{
		this->x /= div;
		this->y /= div;
		this->z /= div;
		return *this;
	}

	Vec3 &operator= (const Vec3 &vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		return *this;
	}

	bool operator== (const Vec3 &vec)
	{
		if (this->x == vec.x)
		{
			if (this->y == vec.y)
			{
				if (this->z == vec.z)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool operator!= (const Vec3 &vec) const
	{
		if (this - x != vec.x)
		{
			return true;
		}
		else if (this - y != vec.y)
		{
			return true;
		}
		else if (this - z != vec.z)
		{
			return true;
		}
		return false;
	}

	bool operator< (const Vec3 &vec) const
	{
		if (this->x >= vec.x)
		{
			return false;
		}
		else if (this->y >= vec.y)
		{
			return false;
		}
		else if (this->z >= vec.z)
		{
			return false;
		}
		return true;
	}

	bool operator> (const Vec3 &vec) const
	{
		if (this->x <= vec.x)
		{
			return false;
		}
		else if (this->y <= vec.y)
		{
			return false;
		}
		else if (this->z <= vec.z)
		{
			return false;
		}
		return true;
	}

	bool operator<= (const Vec3 &vec) const
	{
		if (this->x > vec.x)
		{
			return false;
		}
		else if (this->y > vec.y)
		{
			return false;
		}
		else if (this->z > vec.z)
		{
			return false;
		}
		return true;
	}

	bool operator>= (const Vec3 &vec) const
	{
		if (this->x < vec.x)
		{
			return false;
		}
		else if (this->y < vec.y)
		{
			return false;
		}
		else if (this->z < vec.z)
		{
			return false;
		}
		return true;
	}

	// generic cast operator
	template <typename U>
	operator Vec3<U>()
	{
		return Vec3<U>(static_cast<U>(this->x), static_cast<U>(this->y), static_cast<U>(this->z));
	}
};

#endif // VEC3_HPP

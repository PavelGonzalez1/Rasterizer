#pragma once

#include <string>
namespace Math {
	struct vec3
	{
		union {
			struct { float x, y, z; };
			struct { float r, g, b; };
		};
		
		vec3() : x{ 0 }, y{ 0 }, z{ 0 } {};

		vec3(float val) : x{ val }, y{ val }, z{ val } {};
		vec3(float inX,float inY,float inZ) : x{ inX }, y{ inY }, z{ inZ } {};

		float magnitude() 
		{
			return sqrt(x * x + y * y + z * z);
		}

		vec3 cross(const vec3& other)
		{
			return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
		}

		vec3 normalize()
		{
			return vec3 {x, y ,z} / this->magnitude();
		}

		float dot(const vec3 &other)
		{
			return other.x * x + other.y * y + other.z * z;
		}

		 bool operator==(const vec3& other) const
		{
			return (other.x == x and other.y == y and other.z == z);
		}

		 vec3 operator/(const float other) const
		{
			return { x / other,y / other,z / other };
		}

		 vec3 operator*(const float other) const
		{
			return { x * other,y * other,z * other };
		}

		 vec3 operator-(const vec3& other) const
		{
			return { x - other.x, y - other.y, z - other.z };
		}
		 vec3 operator+(const vec3& other) const
		{
			return { x + other.x, y + other.y, z + other.z };
		}
	};
	
	static std::string to_string(vec3& vec)
	{
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "," ")";
	}
}
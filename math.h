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
	
	struct vec4
	{
		union {
			struct { float x, y, z, w; };
			struct { float r, g, b, w; };
		};

		vec4() : x{}, y{}, z{}, w{} {};

		vec4(float val) : x{ val }, y{ val }, z{ val }, w{ val } {};
		vec4(float inX, float inY, float inZ, float inW) : x{ inX }, y{ inY }, z{ inZ }, w{ inW } {};

		float magnitude()
		{
			return sqrt(x * x + y * y + z * z);
		}

		vec4 normalize()
		{
			return vec4{ x, y ,z , w} / this->magnitude();
		}

		float dot(const vec4& other) const
		{
			return other.x * x + other.y * y + other.z * z;
		}

		bool operator==(const vec4& other) const
		{
			return (other.x == x and other.y == y and other.z == z);
		}

		vec4 operator/(const float other) const
		{
			return { x / other,y / other,z / other , w/other};
		}

		vec4 operator*(const float other) const
		{  
		return { x * other,y * other,z * other, w * other};
		}  
		   
		vec4 operator-(const vec4& other) const
		{  
		return { x - other.x, y - other.y, z - other.z , w - other.w };
		}  
		vec4 operator+(const vec4& other) const
		{
			return { x + other.x, y + other.y, z + other.z, w + other.w };
		}
	};

	struct mat4
	{
		vec4 operator*(const vec4 other) const
		{
			
			return 
			{ 
				(other.x * row[0].x + other.y * row[0].y + other.z * row[0].z + row[0].w * other.w),
				(other.x * row[1].x + other.y * row[1].y + other.z * row[1].z + row[1].w * other.w),
				(other.x * row[2].x + other.y * row[2].y + other.z * row[2].z + row[2].w * other.w),
				(other.x * row[3].x + other.y * row[3].y + other.z * row[3].z + row[3].w * other.w)
			};
		}

		static mat4 setNewAxis(mat4 start, vec3 normal, vec3 up)
		{
			vec3 newZAxis =  normal.normalize();
			vec3 newXAxis = up.cross(newZAxis).normalize();
			vec3 newYAxis = newZAxis.cross(newXAxis);

			start.row[0].x = newXAxis.x;
			start.row[1].x = newXAxis.y;
			start.row[2].x = newXAxis.z;

			start.row[0].y = newYAxis.x; //new y axis
			start.row[1].y = newYAxis.y;
			start.row[2].y = newYAxis.z;

			start.row[0].z = newZAxis.x;
			start.row[1].z = newZAxis.y;
			start.row[2].z = newZAxis.z;

			return start;

		}
		static mat4 translate(mat4 start,vec3 translation)
		{
			start.row[0].w += translation.x;
			start.row[1].w += translation.y;
			start.row[2].w += translation.z;
			return start;
		}

	private:
		vec4 row[4] =
		{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 } 
		};
		
	};
	static std::string to_string(vec3& vec)
	{
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "," ")";
	}
}
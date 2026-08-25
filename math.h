#pragma once

#include <string>
namespace Math {
	const float PI = 3.1416f;
	struct vec3
	{
		union {
			struct { float x, y, z; };
			struct { float r, g, b; };
		};
		
		vec3() : x{ 0 }, y{ 0 }, z{ 0 } {};

		vec3(float val) : x{ val }, y{ val }, z{ val } {};
		vec3(float inX,float inY,float inZ) : x{ inX }, y{ inY }, z{ inZ } {};

		float magnitude() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		vec3 cross(const vec3& other)
		{
			return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
		}

		vec3 normalize() const
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
		vec4(vec3 vec, float inW) : x{ vec.x }, y{ vec.y }, z{ vec.z }, w{ inW } {};

		vec3 xyz() const
		{
			return { x,y,z };
		}
		float magnitude()
		{
			return sqrt(x * x + y * y + z * z);
		}
		vec4 perspectiveDivide()
		{
			float newX = x / w;
			float newY = y / w;
			return vec4( newX, newY,z , w );
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
		vec4 operator*(const vec4& other) const
		{
			
			return 
			{ 
				(other.x * row[0].x + other.y * row[0].y + other.z * row[0].z + row[0].w * other.w),
				(other.x * row[1].x + other.y * row[1].y + other.z * row[1].z + row[1].w * other.w),
				(other.x * row[2].x + other.y * row[2].y + other.z * row[2].z + row[2].w * other.w),
				(other.x * row[3].x + other.y * row[3].y + other.z * row[3].z + row[3].w * other.w)
			};
		}
		mat4 operator*(const mat4& other) const
		{
			mat4 result{};
			
			result.row[0].x = row[0].x * other.row[0].x + row[0].y * other.row[1].x + row[0].z * other.row[2].x + row[0].w * other.row[3].x;
			result.row[0].y = row[0].x * other.row[0].y + row[0].y * other.row[1].y + row[0].z * other.row[2].y + row[0].w * other.row[3].y;
			result.row[0].z = row[0].x * other.row[0].z + row[0].y * other.row[1].z + row[0].z * other.row[2].z + row[0].w * other.row[3].z;
			result.row[0].w = row[0].x * other.row[0].w + row[0].y * other.row[1].w + row[0].z * other.row[2].w + row[0].w * other.row[3].w;

			result.row[1].x = row[1].x * other.row[0].x + row[1].y * other.row[1].x + row[1].z * other.row[2].x + row[1].w * other.row[3].x;
			result.row[1].y = row[1].x * other.row[0].y + row[1].y * other.row[1].y + row[1].z * other.row[2].y + row[1].w * other.row[3].y;
			result.row[1].z = row[1].x * other.row[0].z + row[1].y * other.row[1].z + row[1].z * other.row[2].z + row[1].w * other.row[3].z;
			result.row[1].w = row[1].x * other.row[0].w + row[1].y * other.row[1].w + row[1].z * other.row[2].w + row[1].w * other.row[3].w;

			result.row[2].x = row[2].x * other.row[0].x + row[2].y * other.row[1].x + row[2].z * other.row[2].x + row[2].w * other.row[3].x;
			result.row[2].y = row[2].x * other.row[0].y + row[2].y * other.row[1].y + row[2].z * other.row[2].y + row[2].w * other.row[3].y;
			result.row[2].z = row[2].x * other.row[0].z + row[2].y * other.row[1].z + row[2].z * other.row[2].z + row[2].w * other.row[3].z;
			result.row[2].w = row[2].x * other.row[0].w + row[2].y * other.row[1].w + row[2].z * other.row[2].w + row[2].w * other.row[3].w;

			result.row[3].x = row[3].x * other.row[0].x + row[3].y * other.row[1].x + row[3].z * other.row[2].x + row[3].w * other.row[3].x;
			result.row[3].y = row[3].x * other.row[0].y + row[3].y * other.row[1].y + row[3].z * other.row[2].y + row[3].w * other.row[3].y;
			result.row[3].z = row[3].x * other.row[0].z + row[3].y * other.row[1].z + row[3].z * other.row[2].z + row[3].w * other.row[3].z;
			result.row[3].w = row[3].x * other.row[0].w + row[3].y * other.row[1].w + row[3].z * other.row[2].w + row[3].w * other.row[3].w;
			return result;
			
		}

		static mat4 rotateX(mat4& start, float angleRad)
		{
			mat4 result = mat4();
			float sinVal = sin(angleRad);
			float cosVal = cos(angleRad);
			result.row[0] = { 1,0,0,0 };
			result.row[1] = {0,cosVal,-sinVal,0};
			result.row[2] = {0,sinVal,cosVal,0};
			result.row[3] = { 0,0,0,1 };

			return start * result;

		}
		static mat4 rotateY(mat4& start, float angleRad)
		{
			mat4 result = mat4();
			float sinVal = sin(angleRad);
			float cosVal = cos(angleRad);

			result.row[0] = {cosVal,0,sinVal,0};
			result.row[1] = {0,1,0,0};
			result.row[2] = {-sinVal,0,cosVal,0};
			result.row[3] = {0,0,0,1};

			return start * result;
		}
		static mat4 rotateZ(mat4& start, float angleRad)
		{
			mat4 result = mat4();
			float sinVal = sin(angleRad);
			float cosVal = cos(angleRad);
			
			result.row[0] = {cosVal,-sinVal,0,0};
			result.row[1] = {sinVal,cosVal,0,0};
			result.row[2] = {0,0,1,0};
			result.row[3] = { 0,0,0,1 };

			return   start * result;
		}

		static mat4 perspective(float near, float far, float fovY, float ar)
		{
			mat4 result = mat4();

			
			result.row[0] = {1 / (ar * tan(fovY / 2.0f)), 0, 0, 0};
			result.row[1] = { 0,1/(tan(fovY/2.0f)), 0, 0};
			result.row[2] = { 0, 0, (far + near) / (near - far), -2 * far * near / (near - far) };
			result.row[3] = { 0, 0, -1.0f, 0 };

			return result;
		}
		
		static mat4 translate(mat4& start,const vec3& translation)
		{
			start.row[0].w += translation.x;
			start.row[1].w += translation.y;
			start.row[2].w += translation.z;
			return start;
		}
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
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + ")";
	}
	static std::string to_string(vec4& vec)
	{
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "," + std::to_string(vec.w) +")" + '\n';
	}
	static std::string to_string(mat4 &mat)
	{
		std::string result{};
		for (int i{}; i < 4; i++)
		{
			result += to_string(mat.row[i]);
		}
		return result;
	}
}
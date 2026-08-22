
#include<unordered_map>
#include<iostream>
#include<vector>
#include<array>
#include<string>
#include<algorithm>

#include "math.h"


extern "C"
{
#include"External/qdbmp.h"
}




static bool checkInsideTriangle(int width, int height, Math::vec3 point1, Math::vec3 point2, Math::vec3 point3)
{
	//TODO: Make it work with the z axis
	//current makes the assumption z = 0


	//any pixel with a positive dot product of a perpendicular vector of the line connecting the points should be in if all meet the requirments
	Math::vec3 lineVec1 = point2 - point1;
	Math::vec3 lineVec2 = point3 - point2;
	Math::vec3 lineVec3 = point1 - point3;

	Math::vec3 perpLine1 = Math::vec3(lineVec1.y, -lineVec1.x, 0);
	Math::vec3 perpLine2 = Math::vec3(lineVec2.y, -lineVec2.x, 0);
	Math::vec3 perpLine3 = Math::vec3(lineVec3.y, -lineVec3.x, 0);

	Math::vec3 testPoint = Math::vec3(width, height, 0);
	Math::vec3 testLine1 = point1 - testPoint;
	Math::vec3 testLine2 = point2 - testPoint;
	Math::vec3 testLine3 = point3 - testPoint;

	if (testLine1.dot(perpLine1) >= 0 and
		testLine2.dot(perpLine2) >= 0 and
		testLine3.dot(perpLine3) >= 0)
	{
		return true;
	}
	else return false;

}

enum DataTypes
{
	POSITION,
	COLOR,
};
struct Point {
	int x;
	int y;

	bool operator==(const Point& other) const
	{
		return x == other.x and y == other.y;
	}
};
namespace std {
	template <>
	struct hash<Point> {
		std::size_t operator()(const Point& p) const noexcept {
			
			std::size_t h1 = std::hash<int>{}(p.x);
			std::size_t h2 = std::hash<int>{}(p.y);

			
			return h1 ^ (h2 << 1);
		}
	};
}
class Drawer
{
public:
	Drawer(int width, int height) : m_height{ height }, m_width{ width }, drawingPercision{ 1.0f / (float)std::max(width, height) } {};


	void SubData(std::vector<float> data, DataTypes type);
	void SubData(std::vector<Math::vec3> data, DataTypes type);

	void Points(int count);
	void Lines(int count);
	void Triangles(int count);

	void writeImageToBMP(std::string name);
	std::string getError();
private:

	const static int MAX_SUBDATA{ 2 };
	int m_height;
	int m_width;
	float drawingPercision;  ;
	std::array<std::vector	<float>, MAX_SUBDATA> subDatas;
	std::array<DataTypes, MAX_SUBDATA> subDataType;
	std::string errorMessage;
	int nextSubData{};
	std::unordered_map<Point, Math::vec3> pixels;

	void loadSubData(int count, std::vector<Math::vec3>& pos, std::vector<Math::vec3>& col);

	

	Point NDCtoPoint(Math::vec3 NDC);
};
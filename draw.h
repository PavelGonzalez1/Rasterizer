
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





enum DataTypes
{
	POSITION,
	COLOR,
	TEXCORD
};
struct vertexData
{
	float z = std::uint32_t(-1);
	Math::vec3 color;
};

class Drawer
{
public:
	Drawer(int width, int height) :
		m_height{ height }, m_width{ width },
		drawingPercision{ 1.0f / (float)std::max(width, height) },
		pixels(m_height* m_width )
	{};


	void SubData(std::vector<float> data, DataTypes type);
	void SubData(std::vector<Math::vec3> data, DataTypes type);

	void Points(int count);
	void Lines(int count);
	void Triangles(int count);

	void writeImageToBMP(const char *name);
	std::string getError();

	Math::mat4 model{};
	Math::mat4 projection{};
private:

	const static int MAX_SUBDATA{ 3 };
	const int m_height;
	const int m_width;
	float drawingPercision;

	std::array<std::vector<float>, MAX_SUBDATA> subDatas;

	std::array<DataTypes, MAX_SUBDATA> subDataType;
	std::string errorMessage;
	int nextSubData{};
	std::vector<vertexData> pixels;

	void loadSubData(int count, std::vector<Math::vec4>& pos, std::vector<Math::vec3>& col);
	void drawToMap(Math::vec4 pos, Math::vec3 col);
	float calcZ(float x, float y, Math::vec3& normal, const Math::vec3& point);

	int NDCtoPoint(Math::vec4 NDC);
};
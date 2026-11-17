#ifndef draw_h
#define draw_h
#include<unordered_map>
#include<iostream>
#include<vector>
#include<array>
#include<string>
#include<algorithm>
#include<cstdint>
#include "myMath.h"




enum DataTypes
{
    POSITION = 0,
    COLOR = 1,
    NORMAL = 2
};
struct indexData
{
    unsigned int PosInd;
    unsigned int NormInd;
};
struct vertexData
{
    float z = 1; // max distance
	Math::vec3 color;
    bool real = false;
    Math::vec3 norm;
};

class Drawer
{
public:
	Drawer(int width, int height) :
		m_height{ height }, m_width{ width },
		drawingPercision{ 1.0f / ((float)std::max(width, height)) },
		pixels(m_height* m_width )
	{};


	void SubData(std::vector<float> data, DataTypes type);
	void SubData(std::vector<Math::vec3> data, DataTypes type);

	void Points(int count);
	void Lines(int count);
	void Triangles(int count);
    void IndexTriangles(std::vector<indexData> &indexArr);

    void clear();
    void draw();
	void writeImageToBMP(const char *name);
	std::string getError();

	Math::mat4 model{};
	Math::mat4 projection{};
    Math::vec3 dirLight{};
    
private:

	const static int MAX_SUBDATA{ 3 };
	const int m_height;
	const int m_width;
	float drawingPercision;

	std::array<std::vector<float>, MAX_SUBDATA> subDatas;

	std::string errorMessage;
	int nextSubData{};
	std::vector<vertexData> pixels;

    float area(Math::vec3& screenPoint1, Math::vec3& screenPoint2,Math::vec3& testPoint);
	void drawToMap(Math::vec3 &pos, Math::vec3 &col, Math::vec3 &normalVec);
    void writeTriangle(int posInd, int normInd);
    Math::vec3 NDCtoPoint(const Math::vec4 &NDC);
    
    
};
#endif

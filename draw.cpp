
#include "draw.h"

void Drawer::SubData(std::vector<float> data, DataTypes type)
{
	if (nextSubData >= MAX_SUBDATA)
	{
		errorMessage = "Drawer only supports ";
		errorMessage += std::to_string(MAX_SUBDATA);
		errorMessage += " amount of subdata";
		return;
	}
	for (const auto& d: data) subDatas[nextSubData].push_back(d);

	subDataType[nextSubData] = type;

	nextSubData++;
	
}
void Drawer::SubData(std::vector<Math::vec3> data, DataTypes type)
{
	std::vector<float> newData;
	for (const auto& d : data)
	{
		newData.push_back(d.x);
		newData.push_back(d.y);
		newData.push_back(d.z);
	}
	SubData(newData, type);
}

/// loads intended data into its respective vector
/// defaults to 0,0,0 if there is an error
/// could crash everything/ log an error message, but 
/// this implementation sort of emulates my understanding of openGL
void Drawer::loadSubData(int count,std::vector<Math::vec4>& pos, std::vector<Math::vec3>& col)
{
	int posInd = -1;
	int colInd = -1;

	for (int i{}; i < nextSubData; i++)
	{
		if (subDataType[i] == POSITION)
			posInd = i;
		else if (subDataType[i] == COLOR)
			colInd = i;
	}

	for (int i{}; i < count; i++)
	{
		if (posInd == -1 or (i * 3 + 2) >= subDatas[posInd].size())
			pos.push_back({ 0, 0, 0 ,0});
		else
		{
			Math::vec4 position = projection * model * Math::vec4( subDatas[posInd][i * 3],subDatas[posInd][i * 3 + 1],subDatas[posInd][i * 3 + 2],1.0f );
			
			position = position.perspectiveDivide();

			pos.push_back(position);
		}
		

		if (colInd == -1 or (i * 3 + 2) >= subDatas[colInd].size())
			col.push_back({ 255, 255, 255 });
		else
			col.push_back({ subDatas[colInd][i * 3],subDatas[colInd][i * 3 + 1],subDatas[colInd][i * 3 + 2] });
	}



}
/// <summary>
/// sets the pixels of points to the drawer pixel map
/// </summary>
/// <param name="count">count of verticies</param>
void Drawer::Points(const int count)
{
	std::vector<Math::vec4> positionData;
	std::vector<Math::vec3> colorData;

	loadSubData(count, positionData, colorData);

	for (int i{}; i < count; i++)
	{
		drawToMap(positionData[i], colorData[i]);
	}
}
/// <summary>
/// sets the pixels of lines to the drawer pixel map
/// </summary>
/// <param name="count">count of verticies</param>
//void Drawer::Lines(int count)
//{
//	std::vector<Math::vec4> positionData;
//	std::vector<Math::vec3> colorData;
//
//	loadSubData(count, positionData, colorData);
//
//	
//	//every 2 points calc the distance and the difference vector  
//	count -= count % 2;
//	for (int i{}; i < count; i+=2)
//	{
//		Math::vec3 lineVec = positionData[i] - positionData[i + 1];
//		const float distance = abs(lineVec.magnitude());
//
//		for (float j{}; j <  distance; j += drawingPercision)
//		{
//			float factor = j/distance ; // percent of color outputed from starting vertex; 
//			Math::vec3 curColor = colorData[i] * (1.0f - factor) + colorData[i+1] * factor;
//
//			Math::vec3 curPoint = positionData[i + 1] + lineVec * factor;
//
//
//			drawToMap(curPoint, curColor);
//		}
//
//	}
//}
static bool calcInsideTriangle(const Math::vec4& curPoint, const Math::vec4& position1, const Math::vec4& position2, const Math::vec4& position3, const Math::vec3& normal)
{
	//line vec between points

	Math::vec3 lineVec1 = position2.xyz() - position1.xyz();
	Math::vec3 lineVec2 = position3.xyz() - position2.xyz();
	Math::vec3 lineVec3 = position1.xyz() - position3.xyz();

	//calculate the perpedicular vector

	Math::vec3 perpLine1 = lineVec1.cross(normal);
	Math::vec3 perpLine2 = lineVec2.cross(normal);
	Math::vec3 perpLine3 = lineVec3.cross(normal);

	Math::vec3 testVec1 = curPoint.xyz() - position1.xyz();
	Math::vec3 testVec2 = curPoint.xyz() - position2.xyz();
	Math::vec3 testVec3 = curPoint.xyz() - position3.xyz();

	if (testVec1.dot(perpLine1) >= 0 and testVec2.dot(perpLine2) >= 0 and testVec3.dot(perpLine3) >= 0 )return true;
	return false;
}
float Drawer::calcZ(float x, float y,Math::vec3 &normal,const Math::vec3 &point)
{
	
	return (normal.x * (x - point.x) + normal.y * (y - point.y) - normal.z * point.z) / (-normal.z );
	
}
/// <summary>
/// sets the pixels of triangles to the drawer pixel map
/// </summary>
/// <param name="count">count of verticies</param>
void Drawer::Triangles( int count)
{
	//to compare the w1 and w2 cords to determine the amount of color contributed from each vertex
	Math::vec3 refPoint1 = { 0.0,0.0,0.0f }; 
	Math::vec3 refPoint2 = { 1.0,0.0,0.0f };
	Math::vec3 refPoint3 = { 0.0,1.0,0.0f };
	std::vector<Math::vec4> positionData;
	std::vector<Math::vec3> colorData;

	loadSubData(count, positionData, colorData);


	count -= count % 3;
	for (int i{}; i < count; i += 3)
	{
		//optimization - instead of parsing the entire screen if its the triangle or not it just parses the box around it
		float startX = std::min(positionData[i].x,std::min(positionData[i + 1].x, positionData[i + 2].x));
		float startY = std::min(positionData[i].y,std::min(positionData[i + 1].y, positionData[i + 2].y));
												  
		float endX = std::max(positionData[i].x,  std::max(positionData[i + 1].x, positionData[i + 2].x));
		float endY = std::max(positionData[i].y,  std::max(positionData[i + 1].y, positionData[i + 2].y));

		// calculate the normal to calc the area to determine if a point is in the triangle and to find the plane the triangle is in
		Math::vec3 normalVec = (positionData[i + 2].xyz() - positionData[i].xyz()).cross(positionData[i + 1].xyz() - positionData[i].xyz());
		normalVec = normalVec.normalize(); 

		Math::vec3 triangleLine1 = (positionData[i + 1] - positionData[i]).xyz();
		Math::vec3 triangleLine2 = (positionData[i + 2] - positionData[i]).xyz();
		
		float w1Divisor = (triangleLine1.y) * (triangleLine2.x) - (triangleLine1.x)*(triangleLine2.y);
		float w1, w2;
		Math::vec4 curPoint{};
		Math::vec3 triangleRefLine{};
		

		
		Math::vec3 wCords{};
		for (float y{ startY }; y <= endY; y += drawingPercision)
		{
			for (float x{ startX }; x <= endX; x += drawingPercision)
			{
				
				curPoint = { x,y,calcZ(x,y,normalVec, positionData[i].xyz()) , 1.0};
				//calc the vector from the current point to each vertex
				triangleRefLine = (curPoint - positionData[i]).xyz();

				w2 = (positionData[i].x * (triangleLine2.y) + triangleRefLine.y * triangleLine2.x - curPoint.x * triangleLine2.y) / w1Divisor;
				w1 = (curPoint.y - positionData[i].y - w2 * (triangleLine1.y) ) / (triangleLine2.y);

				
				wCords = { w1,w2,0.f };
				
				float factor2 = (refPoint2 - wCords).magnitude();
				float factor3 = (refPoint3 - wCords).magnitude();
				float factor1 = 1 - factor2 - factor3;
				
				if (w1 >= 0 and w2 >= 0 and w1 + w2 <= 1)
				{
					Math::vec3 curColor = colorData[i] * factor1 + colorData[i + 1] * factor2 + colorData[i + 2] * factor3;
					drawToMap(curPoint, curColor);
				}

			}
		}

	}
}

std::string Drawer::getError()
{
	return this->errorMessage;
}

void Drawer::writeImageToBMP(const char* name)
{
	const unsigned short bitDepth = 24;

	BMP* image = BMP_Create(m_width, m_height, bitDepth);
	for (int y{}; y < m_height; y++)
	{
		for (int x{}; x < m_width; x++)
		{
			vertexData pixel = pixels[y * m_width + x];
			BMP_SetPixelRGB(image, x, y, pixel.color.r, pixel.color.g, pixel.color.b);
		}
	}
	
	


	BMP_WriteFile(image, name );
	BMP_Free(image);
}

int Drawer::NDCtoPoint(Math::vec4 NDC)
{
	
	// [-1,1] -> [0,1];
	float factorX = (NDC.x + (1.0f - drawingPercision)) / 2.0f;
	float factorY = (NDC.y - (1.0f - drawingPercision)) / -2.0f;

	int posX = m_width * factorX;
	int posY = m_height * factorY;

	return posX + posY * m_width;

	
}
void Drawer::drawToMap(Math::vec4 pos, Math::vec3 col)
{	

	
	
	
	if (pos.x < -1 or pos.x > 1 or pos.y > 1 or pos.y < -1)return;

	if (pos.z <  pixels[NDCtoPoint(pos)].z)
	{
		
		pixels[NDCtoPoint(pos)] = { pos.z,col };
	}
}
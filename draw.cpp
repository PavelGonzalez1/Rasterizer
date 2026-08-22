
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
void Drawer::loadSubData(int count,std::vector<Math::vec3>& pos, std::vector<Math::vec3>& col)
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
			pos.push_back({ 0, 0, 0 });
		else
			pos.push_back({ subDatas[posInd][i * 3],subDatas[posInd][i * 3 + 1],subDatas[posInd][i * 3 + 2] });
		

		if (colInd == -1 or (i * 3 + 2) >= subDatas[colInd].size())
			col.push_back({ 0, 0, 0 });
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
	std::vector<Math::vec3> positionData;
	std::vector<Math::vec3> colorData;

	loadSubData(count, positionData, colorData);

	for (int i{}; i < count; i++)
	{
		pixels[NDCtoPoint(positionData[i])] = colorData[i];
	}
}
/// <summary>
/// sets the pixels of lines to the drawer pixel map
/// </summary>
/// <param name="count">count of verticies</param>
void Drawer::Lines(int count)
{
	std::vector<Math::vec3> positionData;
	std::vector<Math::vec3> colorData;

	loadSubData(count, positionData, colorData);

	
	//every 2 points calc the distance and the difference vector  
	count -= count % 2;
	for (int i{}; i < count; i+=2)
	{
		Math::vec3 lineVec = positionData[i] - positionData[i + 1];
		const float distance = abs(lineVec.magnitude());

		for (float j{}; j <  distance; j += drawingPercision)
		{
			float factor = j/distance ; // percent of color outputed from starting vertex; 
			Math::vec3 curColor = colorData[i] * (1.0f - factor) + colorData[i+1] * factor;

			pixels[NDCtoPoint(positionData[i+1]+lineVec * factor)] = curColor;
		}

	}
}
/// <summary>
/// sets the pixels of triangles to the drawer pixel map
/// </summary>
/// <param name="count">count of verticies</param>
void Drawer::Triangles( int count)
{
	std::vector<Math::vec3> positionData;
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

		float wholeArea = (positionData[i + 2] - positionData[i]).cross(positionData[i + 1] - positionData[i]).magnitude()/2.0f;
		
		for (float y{ startY }; y < endY; y += drawingPercision)
		{
			for (float x{ startX }; x < endX; x += drawingPercision)
			{
				const Math::vec3 curPoint = { x,y,0 };

				//calc the vector from the current point to each vertex
				
				Math::vec3 lineVec1 = positionData[i] - curPoint;
				Math::vec3 lineVec2 = positionData[i+1] - curPoint;
				Math::vec3 lineVec3 = positionData[i+2] - curPoint;

				//then calc the area produced for every combination of two lines (cross product / 2)
				// the area / wholeArea is the percent of color that point can have

				float factor1 = lineVec2.cross(lineVec3).magnitude() / 2.0f / wholeArea;
				float factor2 = lineVec3.cross(lineVec1).magnitude() / 2.0f / wholeArea;
				float factor3 = lineVec1.cross(lineVec2).magnitude() / 2.0f / wholeArea;

				// if the percent of every color is greater than 100% the point is outside the triangle
				if (factor1 + factor2 + factor3 <= 1.0f)
				{
					Math::vec3 curColor = colorData[i] * factor1 + colorData[i + 1] * factor2 + colorData[i + 2] * factor3;
					pixels[NDCtoPoint(curPoint)] = curColor;
				}

			}
		}

	}
}

std::string Drawer::getError()
{
	return this->errorMessage;
}

void Drawer::writeImageToBMP(std::string name)
{
	const unsigned short bitDepth = 24;

	BMP* image = BMP_Create(m_width, m_height, bitDepth);

	for (const auto& pixel : pixels)
		BMP_SetPixelRGB(image, pixel.first.x, pixel.first.y, pixel.second.r, pixel.second.g, pixel.second.b);


	BMP_WriteFile(image, (name + ".bmp").c_str());
	BMP_Free(image);
}

Point Drawer::NDCtoPoint(Math::vec3 NDC)
{

	float factorX = (NDC.x + 0.99f) / 2.0f;
	float factorY = (NDC.y - 0.99f) / -2.0f;

	int posX = m_width * factorX;
	int posY = m_height * factorY;

	return { posX,posY };

	
}
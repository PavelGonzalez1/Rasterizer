
#include "draw.h"
#include <chrono> 
const int WIDTH	 = 920;
const int HEIGHT = 1080;
void main() 
{
	auto start = std::chrono::high_resolution_clock::now();

	Drawer drawer = Drawer(WIDTH, HEIGHT);
	std::vector<Math::vec3> posData = { 
		{-1.0f,0.0f,1.0f},
		{1.0f,-1.0f,-1.0f},
		{1.0f,1.0f,-1.0f },

		{-1.0f,-1.0f,0 },
		{1.0f,-1.0f,0},
		{0,1.0f,0}
		

	};
	std::vector<Math::vec3> colData = { {255,0,0},{0,255,0},{0,0,255},{255,0,0},{255,0,0},{255,0,0} };
	drawer.SubData(posData, POSITION);
	drawer.SubData(colData, COLOR);

	drawer.Triangles(6);
	drawer.writeImageToBMP("Triangle.bmp");
	std::cout << drawer.getError() << '\n';

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Time taken by code: " << duration.count() << " microseconds" << std::endl;
}
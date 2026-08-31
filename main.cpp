
#include "draw.h"
#include "External/stb_image.h"
#include <chrono> 
const int WIDTH	 = 1920;
const int HEIGHT = 1080;
void main() 
{
	auto start = std::chrono::high_resolution_clock::now();

	Drawer drawer = Drawer(WIDTH, HEIGHT);
	std::vector<Math::vec3> posData = {
		//back
		{-0.5f,-0.5f,-1.5f},{0.5f, -0.5f,-1.5f},{-0.5f,0.5f,-1.5f},
		{0.5f,-0.5f,-1.5f},{0.5f,0.5f,-1.5f},{-0.5f,0.5f,-1.5f},
		//front
		{-0.5f,-0.5f,-0.5f},{0.5f,-0.5f,-0.5f},{-0.5f,0.5f,-0.5f},
		{0.5f,-0.5f,-0.5f},{0.5f, 0.5f,-0.5f},{-0.5f,0.5f,-0.5f},
	
		//left
		{-0.5f,-0.5f,-1.5f},{-0.5f,-0.5f,-0.5f},{-0.5f,0.5f,-0.5f},
		{-0.5f,-0.5f,-1.5f},{-0.5f,0.5f,-1.5f},{-0.5f,0.5f,-0.5f},
		//right
		{0.5f,-0.5f,-1.5f},{0.5f,-0.5f,-0.5f},{0.5f,0.5f,-0.5f},
		{0.5f,-0.5f,-1.5f},{0.5f,0.5f,-0.5f},{0.5f,0.5f,-1.5f},
		//top
		{-0.5f,0.5f,-1.5f},{0.5f,0.5f,-0.5f},{0.5f,0.5f,-1.5f},
		{-0.5f,0.5f,-1.5f},{-0.5f,0.5f,-0.5f},{0.5f,0.5f,-0.5f},
		//bottom
		{-0.5f,-0.5f,-1.5f},{0.5f,-0.5f,-0.5f},{0.5f,-0.5f,-1.5f},
		{-0.5f,-0.5f,-1.5f},{-0.5f,-0.5f,-0.5f},{0.5f,-0.5f,-0.5f},
	};
	std::vector<Math::vec3> colData = { 
		////back
		{255,255,100},{255,255,100},{255,255,100},
		{255,255,100},{255,255,100},{255,255,100},
		//front
		{255,100,100},{255,100,100},{255,100,100},
		{255,100,100},{255,100,100},{255,100,100},
		
		//left
		{255,100,255},{255,100,255},{255,100,255},
		{255,100,255},{255,100,255},{255,100,255},
		//right
		{100,255,100},{100,255,100},{100,255,100},
		{100,255,100},{100,255,100},{100,255,100},
		//top
		{100,255,255},{100,255,255},{100,255,255},
		{100,255,255},{100,255,255},{100,255,255},
		//bottom
		{100,100,255},{100,100,255},{100,100,255},
		{100,100,255},{100,100,255},{100,100,255},
	};

	std::vector<Math::vec3> texData{

		//back
		{},{},{},
		{},{},{},

		//front
		{},{},{},
		{},{},{},

		//left
		{},{},{},
		{},{},{},

		//right
		{},{},{},
		{},{},{},

		//top
		{},{},{},
		{},{},{},

		//bottom
		{},{},{},
		{},{},{}
	};
	drawer.SubData(posData, POSITION);
	drawer.SubData(colData, COLOR);
	
	drawer.model = Math::mat4::translate(drawer.model, { 0.0f,-1.0,-1.5f });
	drawer.model = Math::mat4::rotateY(drawer.model, -Math::PI / 3);
	drawer.model = Math::mat4::rotateX(drawer.model, Math::PI / 4);
	
	
	
	

	drawer.projection = Math::mat4::perspective(0.5f, 10.0f, Math::PI / 2.f, (float)WIDTH / (float)HEIGHT);
	drawer.Triangles(6 * 6);
	drawer.writeImageToBMP("Triangle.bmp");
	std::cout << drawer.getError() << '\n';
	
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Time taken by code: " << duration.count() << " microseconds" << std::endl;
}

#include "draw.h"

const int WIDTH	 = 100;
const int HEIGHT = 100;
void main() 
{
	Drawer drawer = Drawer(WIDTH, HEIGHT);
	std::vector<Math::vec3> posData = { {-1.0f,-1.0f,0 },{1.0f,-1.0f,0},{0,1.0f,0}, };
	std::vector<Math::vec3> colData = { {255,0,0},{0,255,0},{0,0,255}  };
	drawer.SubData(posData, POSITION);
	drawer.SubData(colData, COLOR);

	drawer.Triangles(3);
	drawer.writeImageToBMP("Triangle");
	std::cout << drawer.getError() << '\n';
}
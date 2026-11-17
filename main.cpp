
#include "draw.h"
#include "objParser.hpp"
#include <chrono>
const int WIDTH	 = 208/2 - 1;
const int HEIGHT = 65;
const int FPS = 60;




int main()
{
    std::string pathToOBJ;
    
    std::cout << "drop file and hit enter\n";
    
    std::cin >> pathToOBJ;
	Drawer drawer = Drawer(WIDTH, HEIGHT);
    
    std::vector<indexData> index;
    std::vector<float> positionData, normalData;
    objParser::parse(pathToOBJ,positionData,normalData,index);
    drawer.SubData(normalData, NORMAL);
    drawer.SubData(positionData, POSITION);
    drawer.dirLight = {0,-0.f,1.f};
    
	
    
	
    
    
	drawer.projection = Math::mat4::perspective(1.0f, 25.0f, Math::PI / 2.f, (float)WIDTH / (float)HEIGHT);
    std::cout << drawer.getError() << '\n';
    
    auto start = std::chrono::high_resolution_clock::now();
    auto frameStart = std::chrono::high_resolution_clock::now();
    while(true)
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::microseconds>(end - frameStart);
        auto duration =std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        if(dt.count() > 1000000/FPS)
        {
            drawer.model = Math::mat4();
            
            drawer.model = Math::mat4::rotateY(drawer.model, (Math::PI / 5) * duration.count() / 1000000.f);
            //drawer.model = Math::mat4::rotateX(drawer.model, (Math::PI / 5) * duration.count() / 1000000.f);
            drawer.model = Math::mat4::translate(drawer.model, { 0.0f,-7.0,-10.5f });
            //
            drawer.IndexTriangles(index);
            drawer.draw();
            drawer.clear();
            frameStart = end;
        }
        
    }
	
	

    return 0;
}

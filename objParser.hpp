//
//  objParser.hpp
//  myRasterizer
//
//  Created by Pavel Gonzalez Del Rio Costa on 9/3/26.
//

#ifndef objParser_hpp
#define objParser_hpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "draw.h"

namespace objParser// only supports triangles :)
{
    static void parse(std::string& path,std::vector<float> &pos, std::vector<float> &norm, std::vector<indexData> &index)
    {
        std::ifstream file(path);
        std::string line;
        if(!file.is_open())
        {
            std::cout << "can't open file" << std::endl;
            return;
        }
        
        while(std::getline(file,line))
        {
            if (line.empty() || line[0] == '#') continue;
            
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;
            if(prefix == "v")
            {
                float v1,v2,v3;
                ss >> v1 >> v2 >> v3;
                pos.push_back(v1);
                pos.push_back(v2);
                pos.push_back(v3);
            }
            else if(prefix == "vn")
            {
                float v1,v2,v3;
                ss >> v1 >> v2 >> v3;
                norm.push_back(v1);
                norm.push_back(v2);
                norm.push_back(v3);
            }
            else if(prefix == "f")
            {
                indexData result;
                std::string face;
                
                
                for(int i{}; i < 3; i++)
                {
                    ss >> face;
                    std::stringstream stream(face);
                    std::string posInd,normInd;
                    std::getline(stream,posInd,'/');
                    std::getline(stream,normInd,'/');
                    std::getline(stream,normInd,'/');
                    
                    try {
                    
                        if(!posInd.empty())result.PosInd = std::stoi(posInd) - 1;
                        if(!normInd.empty())result.NormInd = std::stoi(normInd) - 1;
                    } catch(const std::invalid_argument& e)
                    {
                        std::cout << "Malformed index in face: '" << face << "'" << std::endl;
                        continue;
                    }
                    catch(const std::out_of_range& e)
                    {
                        std::cout << "Index out of range in face: '" << face << "'" << std::endl;
                        continue;
                    }
                    
                    index.push_back(result);
                }
                
            }
        }
    }
}

#endif /* objParser_hpp */

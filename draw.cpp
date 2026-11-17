
#include "draw.h"

const char lightMap[] = {'X','.',':','-','=','+','*','%','#','@'};


void Drawer::SubData(std::vector<float> data, DataTypes type)
{
    for (const auto& d: data) subDatas[type].push_back(d);
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

void Drawer::clear()
{
    std::vector<vertexData>(m_height* m_width).swap( pixels);
}
/// <summary>
/// sets the pixels of triangles to the drawer pixel map
/// </summary>
/// <param name="count">count of verticies</param>
void Drawer::Triangles(int count)
{

    count -= count % 3;
    for (int i{}; i < count; i += 3)
    {
        writeTriangle(i,i);
    }
}
void Drawer::IndexTriangles(std::vector<indexData> &indexes)
{
    for (int i{}; i < indexes.size(); i++)
    {
        writeTriangle(indexes[i].PosInd,indexes[i].NormInd);
    }
}

float Drawer::area(Math::vec3& screenPoint1, Math::vec3& screenPoint2,Math::vec3& testPoint)
{
   return (screenPoint2.x - screenPoint1.x) * (testPoint.y - screenPoint1.y) - (screenPoint2.y - screenPoint1.y) * (testPoint.x - screenPoint1.x);
}


void Drawer::writeTriangle(int posInd, int normInd )
{
    const int PositionStride = 3;
    const int NormalStride = 3;

   
    Math::vec4 point1,point2,point3;
    if(subDatas[POSITION].size() > ((posInd+2) * PositionStride+PositionStride-1))
    {
        point1 =  projection * model * (Math::vec4){subDatas[POSITION][posInd * PositionStride],subDatas[POSITION][posInd * PositionStride+1],
            subDatas[POSITION][posInd * PositionStride+2],1.f};
        if (point1.w <= 1e-4f) return;
        point1 = point1.perspectiveDivide();
        
        point2 = projection * model * (Math::vec4){subDatas[POSITION][(posInd+1) * PositionStride],subDatas[POSITION][(posInd+1) * PositionStride+1],
            subDatas[POSITION][(posInd+1) * PositionStride+2],1.f};
        if (point2.w <= 1e-4f) return;
        point2 = point2.perspectiveDivide();
        
        
        point3 = projection * model * (Math::vec4){subDatas[POSITION][(posInd+2) * PositionStride],subDatas[POSITION][(posInd+2) * PositionStride+1],
            subDatas[POSITION][(posInd+2)* PositionStride+2],1.f};
        if (point3.w <= 1e-4f) return;
        point3 = point3.perspectiveDivide();
        
        
        if((point1.z < 0 & point2.z < 0 & point3.z < 0) || (point1.z > 1 & point2.z > 1 & point3.z > 1) )
        {
            return;
        }
    }
    else
    {
        return;
    }
    Math::vec3 normVec1 , normVec2 , normVec3;
    if(subDatas[NORMAL].size() > ((normInd+2) * NormalStride+NormalStride-1))
    {
        normVec1 = (model * (Math::vec4){subDatas[NORMAL][normInd*NormalStride],subDatas[NORMAL][normInd*NormalStride+1],subDatas[NORMAL][normInd*NormalStride+2],0.f}).xyz();
        normVec2 = (model * (Math::vec4){subDatas[NORMAL][(normInd+1)*NormalStride],subDatas[NORMAL][(normInd+1)*NormalStride+1],subDatas[NORMAL][(normInd+1)*NormalStride+2],0.f}).xyz();
        normVec3 = (model * (Math::vec4){subDatas[NORMAL][(normInd+2)*NormalStride],subDatas[NORMAL][(normInd+2)*NormalStride+1],subDatas[NORMAL][(normInd+2)*NormalStride+2],0.f}).xyz();
    }
    
    

    //convert to point screen space
    float ndcZ[3] = { point1.z,point2.z,point3.z};
    Math::vec3 sp0 = NDCtoPoint(point1);
    Math::vec3 sp1 = NDCtoPoint(point2);
    Math::vec3 sp2 = NDCtoPoint(point3);
    float a = area(sp0, sp1, sp2);
    
    if (a < 1e-8f) return;
    
    float invArea = 1.f / a;
    //optimization - instead of parsing the entire screen if its the triangle or not it just parses the box around it
    int startX =( std::max(std::min(sp0.x,std::min(sp1.x, sp2.x)),0.f));
    int startY =( std::max(std::min(sp0.y,std::min(sp1.y, sp2.y)),0.f));
    int endX =  std::ceil((std::min(std::max(sp0.x,  std::max(sp1.x, sp2.x)),(float)m_width - 1)));
    int endY =  std::ceil((std::min(std::max(sp0.y,  std::max(sp1.y, sp2.y)),(float)m_height - 1)));
    
    
    
    Math::vec3 curPoint{};
    

    
    Math::vec3 wCords{};
    float eps = -1.e-4f;
    for (int y{ startY }; y <= endY; y++)
    {
        for (int x{ startX }; x <= endX; x++)
        {
            
            curPoint = { (float)x ,(float)y,0.f};// center the pixel
            float l0 = area(sp1, sp2, curPoint) * invArea;
            float l1 = area(sp2, sp0, curPoint) * invArea;
            float l2 = 1.f - l0 - l1;
        
            if (l2 >= eps and l1 >= eps and l0 >= eps )
            {
                
                Math::vec3 curNorm = (normVec1 * l0 + normVec2 * l1 + normVec3 *l2).normalize();
                Math::vec3 curColor = {}; //not implemented
                curPoint.z = ndcZ[0] * l0 + ndcZ[1] * l1 + ndcZ[2] * l2;
                if (curPoint.z >= 0 and curPoint.z < 1.f)
                {
                    drawToMap(curPoint, curColor, curNorm);
                }
            }

        }
    }

}
std::string Drawer::getError()
{
    return this->errorMessage;
}
Math::vec3 Drawer::NDCtoPoint(const Math::vec4 &NDC)
{
    float x =( NDC.x + 1.f) / 2.f * (float)m_width;
    float y =( NDC.y - 1.f) / 2.f * (float)-m_height;
    
    return {x,y,0.f};
}

void Drawer::drawToMap(Math::vec3 &pos, Math::vec3 &col, Math::vec3 &normalVec)
{
    
    if (pos.z < pixels[(int)pos.y * m_width + (int)pos.x].z)
    {
        pixels[(int)pos.y * m_width + (int)pos.x] = { pos.z,col , true, normalVec};
    }
}

void Drawer::draw()
{
    const float ambientLightStrength = 0.3;
    const float diffusionLightStrength = 0.7;
    const Math::vec3 lightColor = (Math::vec3){1,1,1}.normalize();
    
    std::string frame;
    frame.reserve(m_width*2 * m_height);
    frame += "\033[H";
    
    for(int y{}; y < m_height; y++)
    {
        for(int x{}; x < m_width; x++)
        {
            Math::vec3 curNorm = pixels[y*m_width+x].norm.normalize();
            if(!pixels[y * (m_width) + x].real)
            {
                frame += "  ";
            }
            else {
                Math::vec3 lightDirection = dirLight.normalize() * 1;
                //calculate brightness to determine what char is representing that pixel
                Math::vec3 ambientLight = lightColor * ambientLightStrength;
                
                Math::vec3 diffusionLight = lightColor * diffusionLightStrength * std::max(curNorm.dot(lightDirection),0.f);
                
                int lightStrength = (ambientLight + diffusionLight).magnitude() * 10.f;
                lightStrength = std::min(lightStrength,9);
                frame += lightMap[(lightStrength)];
                frame += lightMap[(lightStrength)];
            }
            
        }
        frame += "\n";
        
    }
    std::cout << frame << std::flush;
}

//
//  objParser.hpp
//  myRasterizer
//
#ifndef objParser_hpp
#define objParser_hpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "draw.h"

namespace objParser // triangles, quads and n-gons (fan-triangulated)
{
    // Output layout (what Drawer::writeTriangle reads):
    //   pos / norm : every triangle owns 3 consecutive vertices (3 floats each)
    //   index      : ONE entry per triangle, {first vertex of that triangle, first normal of that triangle}
    // OBJ files index vertices individually, so the mesh is "de-indexed" here: shared vertices get duplicated.
    inline void parse(const std::string& path, std::vector<float>& pos, std::vector<float>& norm, std::vector<indexData>& index)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cout << "can't open file: " << path << std::endl;
            return;
        }

        std::vector<float> rawPos, rawNorm;   // as stored in the file
        std::string line;

        // OBJ indices are 1-based, and negative ones count back from the end of the list so far
        auto resolve = [](long i, size_t count) -> long
        {
            if (i > 0) return i - 1;
            if (i < 0) return (long)count + i;
            return -1;
        };

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#') continue;

            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v")
            {
                float x = 0, y = 0, z = 0;
                ss >> x >> y >> z;
                rawPos.insert(rawPos.end(), { x, y, z });
            }
            else if (prefix == "vn")
            {
                float x = 0, y = 0, z = 0;
                ss >> x >> y >> z;
                rawNorm.insert(rawNorm.end(), { x, y, z });
            }
            else if (prefix == "f")
            {
                struct Corner { long p; long n; };   // n == -1 -> this corner has no normal
                std::vector<Corner> face;
                std::string tok;
                bool ok = true;

                // tokens look like "v", "v/vt", "v//vn" or "v/vt/vn"
                while (ss >> tok)
                {
                    std::string p, t, n;
                    std::stringstream st(tok);
                    std::getline(st, p, '/');
                    std::getline(st, t, '/');
                    std::getline(st, n, '/');
                    try
                    {
                        const long pi = resolve(std::stol(p), rawPos.size() / 3);
                        long ni = -1;
                        if (!n.empty()) ni = resolve(std::stol(n), rawNorm.size() / 3);
                        if (pi < 0 || (size_t)pi >= rawPos.size() / 3) { ok = false; break; }
                        if (ni < 0 || (size_t)ni >= rawNorm.size() / 3) ni = -1;
                        face.push_back({ pi, ni });
                    }
                    catch (const std::exception&)
                    {
                        std::cout << "Malformed face vertex: '" << tok << "'" << std::endl;
                        ok = false;
                        break;
                    }
                }
                if (!ok || face.size() < 3) continue;   // drop the whole face, never half of it

                // fan-triangulate: (0,1,2) (0,2,3) (0,3,4) ...
                for (size_t k = 1; k + 1 < face.size(); k++)
                {
                    const Corner tri[3] = { face[0], face[k], face[k + 1] };

                    Math::vec3 P[3];
                    for (int i = 0; i < 3; i++)
                        P[i] = { rawPos[tri[i].p * 3], rawPos[tri[i].p * 3 + 1], rawPos[tri[i].p * 3 + 2] };

                    // flat face normal, used for any corner the file gave no normal for
                    Math::vec3 faceNormal = (P[1] - P[0]).cross(P[2] - P[0]);
                    const float len = faceNormal.magnitude();
                    faceNormal = len > 1e-12f ? faceNormal / len : Math::vec3{ 0, 0, 1 };

                    index.push_back({ (unsigned int)(pos.size() / 3), (unsigned int)(norm.size() / 3) });
                    for (int i = 0; i < 3; i++)
                    {
                        pos.insert(pos.end(), { P[i].x, P[i].y, P[i].z });
                        if (tri[i].n >= 0)
                            norm.insert(norm.end(), { rawNorm[tri[i].n * 3], rawNorm[tri[i].n * 3 + 1], rawNorm[tri[i].n * 3 + 2] });
                        else
                            norm.insert(norm.end(), { faceNormal.x, faceNormal.y, faceNormal.z });
                    }
                }
            }
        }
    }
}

#endif /* objParser_hpp */

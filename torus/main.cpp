#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <vector>

struct Point
{
    double x;
    double y;
    double z;
};

void printTriangle(std::ofstream& outFile,
                   const Point& a, const Point& b, const Point& c)
{
    outFile << "facet normal "
            << (b.y - a.y) * (c.z - a.z) - (b.z - a.z) * (c.y - a.y) << " "
            << (b.z - a.z) * (c.x - a.x) - (b.x - a.x) * (c.z - a.z) << " "
            << (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)
            << std::endl;
    outFile << " outer loop" << std::endl;
    outFile << "  vertex " << a.x << " " << a.y << " " << a.z << std::endl;
    outFile << "  vertex " << b.x << " " << b.y << " " << b.z << std::endl;
    outFile << "  vertex " << c.x << " " << c.y << " " << c.z << std::endl;
    outFile << " endloop" << std::endl;
    outFile << "endfacet" << std::endl;
}

void generateTorus(const char* filename)
{
    const double innerRadius = 1;
    const double outerRadius = 3;
    const int innerCount = 20;
    const int outerCount = 40;

    Point vertices[outerCount][innerCount];

    // Generate a torus mesh
    for (int i = 0; i < outerCount; ++i)
    {
        const double outer = 2.0 * M_PI * i / outerCount;
        for (int t = 0; t < innerCount; ++t)
        {
            const double inner = 2.0 * M_PI * t / innerCount;
            vertices[i][t].x =
                (outerRadius + innerRadius * cos(inner)) * cos(outer);
            vertices[i][t].y =
                (outerRadius + innerRadius * cos(inner)) * sin(outer);
            vertices[i][t].z =
                innerRadius * sin(inner);
        }
    }

    // STL file format
    std::ofstream outFile;
    outFile.open(filename, std::ios::out);

    outFile << std::fixed << std::setprecision(5);
    outFile << "solid torus" << std::endl;
    for (int i = 0; i < outerCount; ++i)
    {
        for (int t = 0; t < innerCount; ++t)
        {
            printTriangle(
                outFile,
                vertices[i][t],
                vertices[(i + 1) % outerCount][t],
                vertices[i][(t + 1) % innerCount]);

            printTriangle(
                outFile,
                vertices[i][(t + 1) % innerCount],
                vertices[(i + 1) % outerCount][t],
                vertices[(i + 1) % outerCount][(t + 1) % innerCount]);
        }
    }
    outFile << "endsolid torus" << std::endl;

    outFile.close();
}

int main(int argc, char* args[])
{
    if (argc == 2)
    {
        generateTorus(args[1]);
    }

    return 0;
}

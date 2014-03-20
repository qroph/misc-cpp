#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <vector>

const double p1 = 0.525731112119133606025669084848;
const double p2 = 0.850650808352039932181540497063;

const double icosahedronVertices[] =
{
    -p1,   0,  p2,     p1,   0,  p2,    -p1,   0, -p2,     p1,   0, -p2,
      0,  p2,  p1,      0,  p2, -p1,      0, -p2,  p1,      0, -p2, -p1,
     p2,  p1,   0,    -p2,  p1,   0,     p2, -p1,   0,    -p2, -p1,   0
};

const int icosahedronIndices[] =
{
    0,  1,  4,     0,  4,  9,     9,  4,  5,     4,  8,  5,     4,  1,  8,
    8,  1, 10,     8, 10,  3,     5,  8,  3,     5,  3,  2,     2,  3,  7,
    7,  3, 10,     7, 10,  6,     7,  6, 11,    11,  6,  0,     0,  6,  1,
    6, 10,  1,     9, 11,  0,     9,  2, 11,     9,  5,  2,     7, 11,  2
};

struct Point
{
    double x;
    double y;
    double z;
};

void addTriangle(std::vector<double>& vertices,
                 const Point& a, const Point& b, const Point& c)
{
    vertices.push_back(a.x);
    vertices.push_back(a.y);
    vertices.push_back(a.z);
    vertices.push_back(b.x);
    vertices.push_back(b.y);
    vertices.push_back(b.z);
    vertices.push_back(c.x);
    vertices.push_back(c.y);
    vertices.push_back(c.z);
}

void divideTriangle(std::vector<double>& vertices,
                    const Point& a, const Point& b, const Point& c,
                    const int& level)
{
    Point ab = {(a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2};
    Point ac = {(a.x + c.x) / 2, (a.y + c.y) / 2, (a.z + c.z) / 2};
    Point bc = {(b.x + c.x) / 2, (b.y + c.y) / 2, (b.z + c.z) / 2};
    const double mab = sqrt(ab.x * ab.x + ab.y * ab.y + ab.z * ab.z);
    const double mac = sqrt(ac.x * ac.x + ac.y * ac.y + ac.z * ac.z);
    const double mbc = sqrt(bc.x * bc.x + bc.y * bc.y + bc.z * bc.z);
    ab.x /= mab;
    ab.y /= mab;
    ab.z /= mab;
    ac.x /= mac;
    ac.y /= mac;
    ac.z /= mac;
    bc.x /= mbc;
    bc.y /= mbc;
    bc.z /= mbc;

    if (level > 1)
    {
        divideTriangle(vertices, a, ab, ac, level - 1);
        divideTriangle(vertices, ab, b, bc, level - 1);
        divideTriangle(vertices, ac, bc, c, level - 1);
        divideTriangle(vertices, ab, bc, ac, level - 1);
    }
    else if (level == 1)
    {
        addTriangle(vertices, a, ab, ac);
        addTriangle(vertices, ab, b, bc);
        addTriangle(vertices, ac, bc, c);
        addTriangle(vertices, ab, bc, ac);
    }
}

void generateSphere(const char* filename)
{
    std::vector<double> vertices;

    // Generate a mesh of sphere
    for (int i = 0; i < 20; ++i)
    {
        const Point a =
            {icosahedronVertices[icosahedronIndices[i * 3] * 3],
             icosahedronVertices[icosahedronIndices[i * 3] * 3 + 1],
             icosahedronVertices[icosahedronIndices[i * 3] * 3 + 2]};
        const Point b =
            {icosahedronVertices[icosahedronIndices[i * 3 + 1] * 3],
             icosahedronVertices[icosahedronIndices[i * 3 + 1] * 3 + 1],
             icosahedronVertices[icosahedronIndices[i * 3 + 1] * 3 + 2]};
        const Point c =
            {icosahedronVertices[icosahedronIndices[i * 3 + 2] * 3],
             icosahedronVertices[icosahedronIndices[i * 3 + 2] * 3 + 1],
             icosahedronVertices[icosahedronIndices[i * 3 + 2] * 3 + 2]};

        divideTriangle(vertices, a, b, c, 3);
    }

    const int vertexCount = vertices.size() / 3;
    const int triangleCount = vertexCount / 3;

    // Add bumps to the sphere
    std::vector<double> heights;
    heights.resize(vertexCount, 1.0);

    for (int i = 0; i < 1500; ++i)
    {
        // A simple way to select a random direction
        Point n;
        do
        {
            n.x = 2.0 * ((double)rand() / RAND_MAX) - 1.0;
            n.y = 2.0 * ((double)rand() / RAND_MAX) - 1.0;
            n.z = 2.0 * ((double)rand() / RAND_MAX) - 1.0;
        }
        while ((n.x * n.x + n.y * n.y + n.z * n.z) > 1);

        const double change = (double)rand() / RAND_MAX;

        for (int t = 0; t < vertexCount; ++t)
        {
            const Point c = {vertices[t * 3],
                             vertices[t * 3 + 1],
                             vertices[t * 3 + 2]};
            const Point cn = {c.x - n.x,
                              c.y - n.y,
                              c.z - n.z};

            const double dot = n.x * cn.x + n.y * cn.y + n.z * cn.z;
            if (dot > 0)
            {
                heights[t] += change / heights[t];
            }
        }
    }

    for (int i = 0; i < vertexCount; ++i)
    {
        vertices[i * 3] *= heights[i];
        vertices[i * 3 + 1] *= heights[i];
        vertices[i * 3 + 2] *= heights[i];
    }

    // STL file format
    std::ofstream outFile;
    outFile.open(filename, std::ios::out);

    outFile << std::fixed << std::setprecision(5);
    outFile << "solid sphere" << std::endl;
    for (int i = 0; i < triangleCount; ++i)
    {
        const Point a = {vertices[i * 9],
                         vertices[i * 9 + 1],
                         vertices[i * 9 + 2]};
        const Point b = {vertices[i * 9 + 3],
                         vertices[i * 9 + 4],
                         vertices[i * 9 + 5]};
        const Point c = {vertices[i * 9 + 6],
                         vertices[i * 9 + 7],
                         vertices[i * 9 + 8]};

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
    outFile << "endsolid sphere" << std::endl;

    outFile.close();
}

int main(int argc, char* args[])
{
    if (argc == 2)
    {
        generateSphere(args[1]);
    }

    return 0;
}

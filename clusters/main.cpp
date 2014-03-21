#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <sstream>

#include "disjoint_sets.h"

// Data points
struct DataPoint: DisjointSetsNode
{
    std::vector<float> attributes;
};
std::vector<DataPoint*> points;
DisjointSets disjointSets;

// A convenience structure to be used with a priority queue
struct DataPointPair
{
    DataPoint* point1;
    DataPoint* point2;
    float distance;

    bool operator<(const DataPointPair& a) const
    {
        return distance > a.distance;
    }
};

// Load data from a file
void loadData(const std::string& filename)
{
    // TODO: Currently this does not do any sanity checks

    // Open a file
    std::ifstream file;
    file.open(filename.c_str());

    while (!file.eof())
    {
        // Read a line
        std::string line;
        getline(file, line);

        if (!line.empty())
        {
            // Create a new data point
            DataPoint* point = new DataPoint();

            // Split the line into tokens
            std::vector<std::string> tokens;
            std::istringstream stream(line);
            std::copy(std::istream_iterator<std::string>(stream),
                      std::istream_iterator<std::string>(),
                      std::back_inserter<std::vector<std::string> >(tokens));

            for (int i = 0; i < (int)tokens.size(); ++i)
            {
                const float attribute = atof(tokens[i].c_str());
                point->attributes.push_back(attribute);
            }

            // Add the data point
            points.push_back(point);
            disjointSets.makeSet(point);
        }
    }

    file.close();
}

// Do the actual clusterization
void clusterize(int clusterCount)
{
    // Generate a priority queue of all pairs
    std::priority_queue<DataPointPair> pairs;

    const int pointCount = points.size();
    for (int i = 0; i < pointCount; ++i)
    {
        for (int j = i + 1; j < pointCount; ++j)
        {
            DataPointPair pair;
            pair.point1 = points[i];
            pair.point2 = points[j];
            pair.distance = 0;

            // Calculate a distance between the points
            const int attributeCount = points[0]->attributes.size();
            for (int a = 0; a < attributeCount; ++a)
            {
                pair.distance += (points[i]->attributes[a] -
                                  points[j]->attributes[a]) *
                                 (points[i]->attributes[a] -
                                  points[j]->attributes[a]);
            }

            pairs.push(pair);
        }
    }

    // Merge the clusters
    int currentClusterCount = points.size();
    while (currentClusterCount > clusterCount)
    {
        // Get points/clusters that have the smallest distance
        const DataPointPair pair = pairs.top();
        pairs.pop();

        if (!disjointSets.sameComponent(pair.point1, pair.point2))
        {
            disjointSets.uniteSets(pair.point1, pair.point2);
            --currentClusterCount;
        }
    }
}

// Show current clusters
void showClusterInformation()
{
    std::map<DisjointSetsNode*, int> clusters;

    const int pointCount = points.size();
    for (int i = 0; i < pointCount; ++i)
    {
        ++clusters[disjointSets.findSet(points[i])];
    }

    std::cout << "Cluster sizes:" << std::endl;

    int current = 0;
    std::map<DisjointSetsNode*, int>::iterator iter;
    for (iter = clusters.begin(); iter != clusters.end(); ++iter)
    {
        ++current;
        std::cout << "  Cluster " << current << ": "
                  << iter->second << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: clusters datafile cluster_count" << std::endl;
        return 1;
    }

    loadData(argv[1]);

    std::cout << "Number of data points: "
              << points.size() << std::endl;
    std::cout << "Number of attributes: "
              << points[0]->attributes.size() << std::endl;

    clusterize(atoi(argv[2]));

    showClusterInformation();

    return 0;
}

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sys/time.h>

using namespace std;

class StopWatch
{
  private:
    int millis;

  public:
    void start()
    {
        timeval tp;
        gettimeofday(&tp, nullptr);
        millis = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    }

    int get_elapsed_ms()
    {
        timeval tp;
        gettimeofday(&tp, nullptr);
        return tp.tv_sec * 1000 + tp.tv_usec / 1000 - millis;
    }
};

class FileHandler
{
  public:
    map<int, vector<int>> readDataFromFile(string filename)
    {
        ifstream infile(filename, ios::in);
        string line;
        int node1;
        int node2;
        map<int, vector<int>> data;
        //discard first 2 elements in the file
        infile >> line;
        infile >> line;
        while (!infile.eof())
        {
            infile >> line;
            node1 = stoi(line);
            vector<int> adjNodes;
            data.insert(make_pair(node1, adjNodes));
            infile >> line;
            node2 = stoi(line);
            adjNodes = data[node1];
            adjNodes.push_back(node2);
            data[node1] = adjNodes;
            adjNodes = data[node2];
            adjNodes.push_back(node1);
            data[node2] = adjNodes;
        }
        for (map<int, vector<int>>::iterator iterator = data.begin(); iterator != data.end(); iterator++)
        {
            vector<int> adjNodes = data[iterator->first];
            for (int i = adjNodes.size() - 1; i >= 0; i--)
            {
                if (adjNodes[i] == iterator->first)
                {
                    adjNodes.erase(adjNodes.begin() + i);
                }
            }
            data[iterator->first] = adjNodes;
            cout << iterator->first << endl;
            for (int i = 0; i < adjNodes.size(); i++)
            {
                cout << adjNodes[i] << " ";
            }
            cout << endl
                 << endl;
        }
        infile.close();
        return data;
    }

    void writeOutputToFile(string filename, map<int, int> coloredVertices)
    {
        ofstream outfile(filename, ios::out);
        for (int i = 0; i < coloredVertices.size(); i++)
        {
            outfile << i << " " << coloredVertices[i] << endl;
        }
        outfile.close();
    }

    void writeRunningTimeToFile(string filename, int runtime)
    {
        ofstream outfile(filename, ios::out);
        outfile << "Running time in ms: " << runtime << endl;
        outfile.close();
    }
};

map<int, int> greedyColorVertices(map<int, vector<int>> data);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout << "Error! missing arguments" << endl;
        exit(-1);
    }

    string infile = argv[1];
    string outfile = argv[2];
    string runningTimeFile = argv[3];

    StopWatch *stopWatch = new StopWatch();
    FileHandler *fileHandler = new FileHandler();
    stopWatch->start();
    map<int, vector<int>> data = fileHandler->readDataFromFile(infile);

    map<int, int> coloredVertices = greedyColorVertices(data);
    int running_time = stopWatch->get_elapsed_ms();
    fileHandler->writeOutputToFile(outfile, coloredVertices);
    fileHandler->writeRunningTimeToFile(runningTimeFile, running_time);
    delete fileHandler;
    delete stopWatch;
    return 0;
}

map<int, int> greedyColorVertices(map<int, vector<int>> data)
{
    // a map that holds the vertex and its corresponding color
    map<int, int> coloredVertecies;

    //a vector to loop on all adjacent nodes of a specific vertex
    vector<int> adjNodes;
    vector<int> vertexColor;
    bool availableColors[data.size()];
    for (int i = 0; i < data.size(); i++)
    {
        availableColors[i] = false;
    }

    //assign a color for the first node in the graph
    vertexColor.push_back(0);
    //assign all nodes the same color initially all are zeros
    for (int i = 1; i < data.size(); i++)
    {
        vertexColor.push_back(-1);
    }

    int color;

    // loop on all vertices assign color to each vertex
    for (int v = 1; v < data.size(); v++)
    {
        //get the vector of all adjacent nodes
        adjNodes = data[v];
        for (int k = 0; k < adjNodes.size(); k++)
        {
            if (vertexColor[adjNodes[k]] != -1)
            {
                availableColors[vertexColor[adjNodes[k]]] = true;
            }
        }
        for (color = 0; color < v; color++)
        {
            if (availableColors[color] == false)
                break;
        }
        vertexColor[v] = color;
        for (int i = 0; i < adjNodes.size(); i++)
        {
            if (vertexColor[adjNodes[i]] != -1)
            {
                availableColors[vertexColor[adjNodes[i]]] = false;
            }
        }
    }

    for (int i = 0; i < vertexColor.size(); i++)
    {
        coloredVertecies.insert(make_pair(i, vertexColor[i]));
    }
    return coloredVertecies;
}
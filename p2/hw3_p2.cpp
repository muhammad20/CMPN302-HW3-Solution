#include <iostream>
#include <string>
#include <sys/time.h>
#include <vector>
#include <sstream>
#include <fstream>

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

vector<string> splitString(string stringToSplit, char delimiter)
{
    vector<string> splittedString;
    stringstream ss(stringToSplit);
    string item;
    while (getline(ss, item, delimiter))
    {
        splittedString.push_back(item);
    }
    return splittedString;
}

class FileHandler
{
  public:
    long **readArrayFromFile(string filename)
    {
        string line;
        ifstream infile(filename, ios::in);

        // get the array size
        infile >> line;
        long data_size = stoi(line);

        //get the data in a single string then split it/
        long *data_arr = new long[data_size];
        for (long i = 0; i < data_size; i++)
        {
            infile >> line;
            data_arr[i] = stoi(line);
        }

        //close the file
        infile.close();

        long **data = new long *[2];
        data[0] = &data_size;
        data[1] = data_arr;
        return data;
    }

    void writeHeightToFile(string filename, long height)
    {
        ofstream outfile(filename, ios::out);
        outfile << height << endl;
        outfile.close();
    }

    void writeRunningTimeToFile(string filename, long runningTime)
    {
        ofstream outfile(filename, ios::out);
        outfile << "Running time in ms: " << runningTime << endl;
        outfile.close();
    }
};
int getHeight(long *data, int pointingIndex, int height, long *heights);
int getMaxHeight(long *, long);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout << "error! missing arguments\n";
        exit(1);
    }

    string infile = argv[1];
    string outfile = argv[2];
    string running_time_file = argv[3];

    FileHandler *fileHandler = new FileHandler();
    long **data = fileHandler->readArrayFromFile(infile);
    long size = *data[0];
    long *tree = data[1];

    StopWatch *stopWatch = new StopWatch();
    stopWatch->start();
    int height = getMaxHeight(tree, size);
    int running_time = stopWatch->get_elapsed_ms();
    fileHandler->writeHeightToFile(outfile, height);
    fileHandler->writeRunningTimeToFile(running_time_file, running_time);
    delete fileHandler;
    delete stopWatch;
    /// --------------- for testing and debugging
    //string filename = "/home/engineer/Desktop/CCE-C/Senior-1/Fall18/CMPN302/Assignments/HW3/p2/p2.in";

    // FileHandler *fileHandler = new FileHandler();
    // long **data = fileHandler->readArrayFromFile(filename);
    // long size = *data[0];
    // long *tree = data[1];
    // StopWatch *stopWatch = new StopWatch();
    // int height = getHeight(tree, size);

    // cout << height << endl;

    // fileHandler->writeHeightToFile("p2.out", height);

    // delete fileHandler;
    // delete stopWatch;
    // delete[] data[1];
    return 0;
}

int getMaxHeight(long *data, long size)
{
    int maxHeight = 0;
    int current_height;
    long heights[size];
    for (int i = 0; i < size; i++)
    {
        heights[i] = 0;
    }
    for (int i = 0; i < size; i++)
    {
        current_height = getHeight(data, i, 1, heights);
        if (current_height >= maxHeight)
        {
            maxHeight = current_height;
        }
    }
    return maxHeight;
}

int getHeight(long *data, int pointingIndex, int height, long *heights)
{
    if (heights[pointingIndex] != 0)
    {
        return heights[pointingIndex];
    }
    if (data[pointingIndex] == -1)
    {
        return 1;
    }

    heights[pointingIndex] = 1 + getHeight(data, data[pointingIndex], ++height, heights);
    return heights[pointingIndex];
}
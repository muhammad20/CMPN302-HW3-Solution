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
    long **readDataFromFile(string filename)
    {
        ifstream infile(filename, ios::in);
        string inData;
        infile >> inData;
        long count = stoi(inData);
        long *data = new long[count];
        for (int i = 0; i < count; i++)
        {
            infile >> inData;
            data[i] = stoi(inData);
        }
        infile.close();

        long **returnData = new long *[2];
        returnData[0] = &count;
        returnData[1] = data;
        return returnData;
    }

    void writeResultToFile(string filename, long output)
    {
        ofstream outfile(filename, ios::out);
        outfile << output << endl;
        outfile.close();
    }

    void writeRunningTimeToFile(string filename, int runtime)
    {
        ofstream outfile(filename, ios::out);
        outfile << "Running time in ms: " << runtime << endl;
        outfile.close();
    }
};

long getMaxProfit(long *data, int size);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout << "Error! missing arguments!\n";
        exit(-1);
    }
    string infile = argv[1];
    string outfile = argv[2];
    string runningTimeFile = argv[3];
    FileHandler *fileHandler = new FileHandler();
    long **data = fileHandler->readDataFromFile(infile);

    int size = (int)*data[0];
    long *coins = data[1];
    StopWatch *stopWatch = new StopWatch();
    stopWatch->start();
    long maxProfit = getMaxProfit(coins, size);
    int running_time = stopWatch->get_elapsed_ms();
    fileHandler->writeResultToFile(outfile, maxProfit);
    fileHandler->writeRunningTimeToFile(runningTimeFile, running_time);
    delete fileHandler;
    delete stopWatch;
    return 0;
}

long getMaxProfit(long *coins, int size)
{
    //the sum that the user have got till now
    long sum = 0;

    // a boolean array to check if the coin at index i is chosen.
    bool isChosen[size];
    long game[size][size];
    for (int i = 0; i < size; i++)
    {
        isChosen[i] = false;
    }
    long player1_choice;
    long player2_choice;
    int index;
    long x, y, z;
    for (int k = 0; k < size; k++)
    {
        for (int i = 0, j = k; j < size; j++, i++)
        {
            if (i + 2 <= j)
                x = game[i + 2][j];
            else
                x = 0;

            if (i + 1 <= j - 1)
                y = game[i + 1][j - 1];
            else
                y = 0;

            if (i <= j - 2)
                z = game[i][j - 2];
            else
                z = 0;

            game[i][j] = max(coins[i] + min(x, y), coins[j] + min(y, z));
        }
    }

    return game[0][size - 1];
}

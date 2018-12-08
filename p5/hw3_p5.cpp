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

class FileHandler
{
  public:
    string readStringFromFile(string filename)
    {
        ifstream infile(filename, ios::in);
        string data;
        // discard the number of characters since we will deal with `strings`.
        getline(infile, data);

        //get the actual string
        getline(infile, data);
        infile.close();
        return data;
    }

    void writeResultToFile(string filename, string palindrome)
    {
        ofstream outfile(filename, ios::out);
        outfile << palindrome.size() << endl;
        outfile << palindrome << endl;
        outfile.close();
    }

    void write_running_time_file(string filename, int running_time)
    {
        ofstream outfile(filename, ios::out);
        outfile << "Running time in ms: " << running_time << endl;
        outfile.close();
    }
};

string getLongestPalindrome(string);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout << "error! missing arguments\n";
        exit(1);
    }
    string infile = argv[1];
    string outfile = argv[2];
    string runningTimFile = argv[3];
    FileHandler *fileHandler = new FileHandler();
    string data = fileHandler->readStringFromFile(infile);

    StopWatch *stopWatch = new StopWatch();
    stopWatch->start();

    string longestPalindrome = getLongestPalindrome(data);
    int running_time = stopWatch->get_elapsed_ms();
    fileHandler->writeResultToFile(outfile, longestPalindrome);
    fileHandler->write_running_time_file(runningTimFile, running_time);
    delete fileHandler;
    delete stopWatch;
    return 0;
}


string getLongestPalindrome(string data)
{
    int size = data.size();
    bool *palTable[size];

    for (int i = 0; i < data.size(); i++)
    {
        palTable[i] = new bool[data.size()];
        for (int j = 0; j < data.size(); j++)
        {
            if (i == j)
            {
                palTable[i][j] = true;
            }
            else
            {
                palTable[i][j] = false;
            }
        }
    }

    //all substrings of size 1 are palindromes
    for(int i = 0; i < size; i++)
    {
        palTable[i][i] = true;
    }

    int maxLength = 1;
    int longestPalStart = 0;
    for(int i = 0; i < size - 1; i++)
    {
        if(data[i] == data[i + 1])
        {
            palTable[i][i + 1] = true;
            maxLength = 2;
            longestPalStart = i;
        }
    }

    for(int i = 3; i <= size; i++)
    {
        for(int j = 0; j < size - i + 1; j++)
        {
            int k = i + j - 1;
            if(palTable[j + 1][k - 1] && data[j] == data[k])
            {
                palTable[j][k] = true;
                if(i > maxLength)
                {
                    longestPalStart = j;
                    maxLength = i;
                }
            }
        }
    }

    return data.substr(longestPalStart, maxLength);
}
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>


using namespace std;
struct Point
{
    int dx, dy;
};

vector<vector<Point>> shapeDefs =
{
    {{0,0}, {1,0}, {2,0}, {0,1}, {1,1}, {2,1}, {2,2}}, // 0: ###, ###, ..#
    {{0,0}, {1,0}, {1,1}, {2,1}, {0,2}, {1,2}, {2,2}}, // 1: ##., .##, ###
    {{0,0}, {1,0}, {2,0}, {2,1}, {0,2}, {1,2}, {2,2}}, // 2: ###, ..#, ###
    {{0,0}, {1,0}, {2,0}, {1,1}, {0,2}, {1,2}, {2,2}}, // 3: ###, .#., ###
    {{0,0}, {1,0}, {2,0}, {0,1}, {1,1}, {0,2}},       // 4: ###, ##., #..
    {{2,0}, {1,1}, {2,1}, {0,2}, {1,2}}               // 5: ..#, .##, ##.
};

bool isWorstCaseSmaller(vector<int>& counts, int w, int h)
{
    long long area = (w * h);
    long long sumShapes = 0;
    for(auto numberShapes: counts)
    {
        sumShapes += (numberShapes * 9);
        if(sumShapes > area)
            return false;
    }

    return true;
}

/*
Part one: just checking if they even possibly could fit

*/
int main()
{
    int countFittingRegions = 0;
    ifstream infile("input.txt");
    if (!infile)
    {
        cout << "You fucked it up!" << endl;
        return 1;
    }

    string line;
    while (getline(infile, line))
    {
        if (line.find('x') == string::npos || line.find(':') == string::npos)
            continue;

        string mod = line;
        for(char &c : mod)
        {
            if(c == 'x' || c == ':')
                c = ' ';
        }

        stringstream ss(mod);
        int w, h;
        vector<int> counts(6);

        if (ss >> w >> h >> counts[0] >> counts[1] >> counts[2] >> counts[3] >> counts[4] >> counts[5])
        {
            bool  boolIsWorstCaseSmaller = isWorstCaseSmaller(counts, w, h);
            if(!boolIsWorstCaseSmaller)
                continue;
            if(boolIsWorstCaseSmaller) //isWorstCaseSmaller(counts, w, h))
            {
                countFittingRegions++;
                cout << "SUCCESS: " << line << " FittingRegions " << countFittingRegions << endl;
                continue;
            }
        }
    }

    return 0;
}

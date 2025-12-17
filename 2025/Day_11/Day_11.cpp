#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <queue>
#include <set>
#include <algorithm>
#include <map>


using namespace std;


map<string, vector<string>> devicesOutputs;
map<pair<string, string>, long long> memo3;

struct PathState
{
    string device;
    bool dacHit;
    bool fftHit;

    bool operator<(const PathState& other) const
    {
        if (device != other.device) 
            return device < other.device;
        if (dacHit != other.dacHit) 
            return dacHit < other.dacHit;
        return fftHit < other.fftHit;
    }
};

map<PathState, long long> memo;

vector<string> splitString(string str, string delimiter)
{
    vector<string> v;
    if (!str.empty())
    {
        int start = 0;
        do
        {
            int idx = str.find(delimiter, start);
            if (idx == string::npos)
                break;

            int length = idx - start;
            v.push_back(str.substr(start, length));
            start += (length + delimiter.size());
        }
        while (true);
        v.push_back(str.substr(start));
    }
    return v;
}

std::vector<std::string> split(const std::string& str)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token)
        tokens.push_back(token);

    return tokens;
}

//PART 2
long long pathCountP2(string key, bool dacVisited, bool fftVisited)
{
    long long antPathes = 0;
    vector<string> adjNodes = devicesOutputs[key];
    //BASE CASE: "out" is part of value-vector
    bool isOut = false;

    if(key == "dac" || dacVisited)
        dacVisited = true;

    if(key == "fft" || fftVisited)
        fftVisited = true;

    PathState currentState = {key, dacVisited, fftVisited};

    if (memo.count(currentState))
        return memo[currentState];

    for(auto adjNode : adjNodes)
    {
        if(adjNode == "out")
        {
            if(dacVisited && fftVisited)
                antPathes++;
        }

    }

    for(auto adjNode : adjNodes)
    {
        if(adjNode == "out")
            continue;

        antPathes += pathCountP2(adjNode, dacVisited, fftVisited);
    }
    memo[currentState] = antPathes;
    return antPathes;
}

long long pathCount3(string key, string stopvalue)      //DFS
{

    long long antPathes = 0;
    vector<string> adjNodes = devicesOutputs[key];

    pair<string, string> currentKey = {key, stopvalue};

    if (memo3.count(currentKey))
        return memo3[currentKey];

    for(auto adjNode : adjNodes)
    {
        if(adjNode == stopvalue)
                antPathes++;
    }

    for(auto adjNode : adjNodes)
    {
        if(adjNode == stopvalue)
            continue;

        antPathes += pathCount3(adjNode, stopvalue);
    }

    memo3[currentKey] = antPathes;
    return antPathes;
}

//PART 1
int pathCount(string key)
{
    int antPathes = 0;
    vector<string> adjNodes = devicesOutputs[key];
    //BASE CASE: "out" is part of value-vector
    bool isOut = false;

    for(auto adjNode : adjNodes)
    {
        if(adjNode == "out")
            antPathes++;
    }

    for(auto adjNode : adjNodes)
    {
        if(adjNode == "out")
            continue;

        antPathes += pathCount(adjNode);
    }

    return antPathes;
}


int main()
{
    fstream f("input.txt");

    if(!f)
        cout << "Problem med fil " << endl;

    string line;

    while(getline(f, line))
    {
        vector<string> splitLines = split(line);

        cout << line << endl;

        if(splitLines[0].back() == ':')
            splitLines[0].pop_back();

        string key = splitLines[0];
        splitLines.erase(splitLines.begin());

        devicesOutputs[key] = splitLines;
    }
    f.close();
    cout << "DevicesOutput size " << devicesOutputs.size() << endl;

    vector<string> youNodes = devicesOutputs["fft"];
    for(auto youNode : youNodes)
        cout << youNode << " ";

        cout << " --- \n" << endl;

    int r = pathCount("you");
    cout << "Part 1: " << r << endl;

    bool dac = false;
    bool fft = false;
//    cout << " " << pathCountP2("svr", dac, fft) << endl;
    long long s = pathCountP2("svr", dac, fft);
    cout << "Part 2: " << s << endl;

    return 0;
}

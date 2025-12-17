#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <queue>
#include <set>
#include <algorithm>
#include <map>

#define MAX 141
//#define MAX 16

using namespace std;
//Start 2 70

char grid[MAX][MAX];

void printPath(const vector<pair<int, int>>& path)
{
    cout << "Path: ";
    for (const auto& p : path)
    {
        cout << "(" << p.first << ", " << p.second << ") -> ";
    }
    cout << "END" << endl;
}

int getNext(pair<int, int> p, pair<int, int>& rPair, int leftRight) //left: leftRight -1; right: leftRight +1
{

    int col = p.second + leftRight;
    if (col < 0 || col >= MAX)
        return 0;

    int i = p.first;

    while (i < MAX)
    {
        if (grid[i][col] == '^')
        {
            rPair = {i, col};
            return 1; // Found the splitter
        }
        i++;
    }
    return 0;
}

int bfsSplit(pair<int, int> start, set<pair<int, int>>& visited)
{
    int splits = 0;
    pair<int, int> startTemp = start;     //Get real start position
    getNext(startTemp, start, 0);

    queue<pair<int, int>> q;
    q.push(start);
    visited.insert(start);

    while (q.empty() == false)
    {
        pair<int, int> node = q.front();

        splits++;
        q.pop();

        pair<int, int> explLeft;
        pair<int, int> explRight;

        if(getNext(node, explLeft, -1) > 0)                 //Explore left side
        {
            if (visited.find(explLeft) == visited.end())
            {
                q.push(explLeft);
                visited.insert(explLeft);
            }
        }
        if(getNext(node, explRight, +1) > 0)                 //Explore right side
        {
            if (visited.find(explRight) == visited.end())   //IF explRight !isE in visited:
            {                                               //mark as visited
                q.push(explRight);
                visited.insert(explRight);
            }
        }
    }

    return splits;
}


long long dfsSplit(pair<int, int> node, map<pair<int, int>, long long>& memo)
{

    if (memo.count(node))       //Memoization
             return memo[node];

    pair<int, int> explLeft;
    pair<int, int> explRight;

    bool canGoLeft = (getNext(node, explLeft, -1) > 0);
    bool canGoRight = (getNext(node, explRight, +1) > 0);

    long long pathsFromThisNode = 0;

    if (canGoLeft) // Add the total paths found by the recursive call
        pathsFromThisNode += dfsSplit(explLeft, memo);
    else                                                    //Base case 1: cannot go left
        pathsFromThisNode += 1;

    if (canGoRight)
        pathsFromThisNode += dfsSplit(explRight, memo);
    else
        pathsFromThisNode += 1;                             //Base case 2: cannot go right

    return memo[node] = pathsFromThisNode;
}

long long partTwo(pair<int, int> start)
{
    map<pair<int, int>, long long> memo;
    vector<pair<int, int>> initialPath = {start};

    pair<int, int> startTemp = start;
    getNext(startTemp, start, 0);

    long long timelines = 0;
    timelines = dfsSplit(start, memo);
    return timelines;
}


int main()
{
    set<pair<int, int>> visited;
    vector<pair<int, int>> splitterPositions;
    pair<int, int> startPos;


    ifstream f("input.txt");
    if(!f)
        cout << "You fucked it up!" << endl;

    char ch;
    int counteruniquePositions = 0;
    string line;
    int i = 0, j = 0;

    do
    {
        f.get(ch);
        grid[i][j] = ch;

        if((ch != '.') && (ch != '\n'))
        {

            pair<int, int> aktuellPos = {i, j};


            if(ch == '^')
                splitterPositions.push_back(aktuellPos);
            if(ch == 'S')
                startPos = aktuellPos;
        }

        i += (ch == '\n') ? 1 : 0;
        j =  (ch == '\n') ? 0 : j+1;

    }while(!f.eof());
    f.close();

    sort(splitterPositions.begin(), splitterPositions.end());

    int partOneRes = bfsSplit(startPos, visited);

    long long partTwoRes = partTwo(startPos);

    cout << "Part 1: " << partOneRes << endl;
    cout << "Part 2: " << partTwoRes << endl;

    return 0;
}

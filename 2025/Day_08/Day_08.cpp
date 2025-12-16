#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>

#define MAX_PART_1 1000


using namespace std;

struct point
{
    long long x;
    long long y;
    long long z;

    bool operator<(const point& other) const
    {
        if (x != other.x)
        {
            return x < other.x;
        }
        if (y != other.y)
        {
            return y < other.y;
        }
        return z < other.z;
    }
    bool operator==(const point& other) const
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }

    bool operator!=(const point& other) const
    {
        return !(*this == other);
    }
};

struct dxPoint
{
    point p1;
    point p2;
    long long d;
};

bool cmpDistances(dxPoint a, dxPoint b) //Comparator function to compare Points by distance
{
    return a.d < b.d;
}

vector<point> vecPoints;
vector<dxPoint> vecDistances;
set<point> connectedPoints;
vector<set<point>> circuits;


long long getDistance(point a, point b)
{
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    long long dz = a.z - b.z;

    return dx * dx + dy * dy + dz * dz;
}

vector<string> split(string str, string delimiter)
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


void printPoint(const point& p)
{
    cout << "(" << p.x << "," << p.y << "," << p.z << ") ";
}

int isInCircuit(point p) //Returns index (vector of Sets) OR -1 if not found
{
        for(auto indx = 0; indx < circuits.size(); indx++)
        {
            if(circuits[indx].count(p))
                return indx;
        }
        return -1;
}

void partTwo()
{
    queue<dxPoint> dxQ;
    circuits.clear();

    for(dxPoint dx : vecDistances)
        dxQ.push(dx);

    for(const point& p : vecPoints)     //Generate Single Element Circuits
    {
        set<point> singleElementSet;
        singleElementSet.insert(p);
        circuits.push_back(singleElementSet);
    }

    int counter = 0;
    cout << "Queue size " << dxQ.size() << endl;
    while(!dxQ.empty())     //Get next distance/points from queue
    {
        dxPoint dxP = dxQ.front();
        dxQ.pop();

        // 1. Find the circuits (components) for both points
        int p1_idx = isInCircuit(dxP.p1);
        int p2_idx = isInCircuit(dxP.p2);


        if (p1_idx == -1 && p2_idx == -1)
        {
            //P1 not connected AND P2 not connected -> Connect, new circuit
            set<point> newCircuit;

            newCircuit.insert(dxP.p1);
            newCircuit.insert(dxP.p2);
            circuits.push_back(newCircuit);

            counter++;
        }
        else if (p1_idx != -1 && p2_idx == -1)
        {
            //P1 connected to a circuit, P2 not connected -> Add P2 to P1:s circuit
            circuits[p1_idx].insert(dxP.p2);
            counter++;
        }
        else if (p1_idx == -1 && p2_idx != -1)
        {
            //P2 connected to a circuit, P1 not connected -> Add P1 to P2:s circuit
            circuits[p2_idx].insert(dxP.p1);
            counter++;
        }
        else if (p1_idx == p2_idx)
        {
            //P1 AND P2 connected to the same circle -> Gå vidare med livet. Lol!
            continue;
        }
        else //Annars, då...?!?
        {
            //P1 is connected to one circuit, P2 is connected to another circuit -> Merge OwO
            int p_small = min(p1_idx, p2_idx);
            int p_large = max(p1_idx, p2_idx);

            circuits[p_small].insert(circuits[p_large].begin(), circuits[p_large].end());
            circuits.erase(circuits.begin() + p_large); //There can be only one! Mwuahahaha!
            counter++;
        }

        //PART 2 - Output
        if(circuits.size() == 1)
        {
            cout << " circuits size 111" << circuits.size() << endl;

            cout << "P1 ";
            printPoint(dxP.p1);
            cout << "P2 ";
            printPoint(dxP.p2);
            cout << endl;

            cout << "PART 2 " << dxP.p1.x * dxP.p2.x << endl;
         }
    }
    return;
}

void partOne()
{

    queue<dxPoint> dxQ;

     for(dxPoint dx : vecDistances)
    {
        dxQ.push(dx);

        //PART ONE
        if(dxQ.size() == MAX_PART_1-1)           //MAX - 1
            break;
    }

    for(const point& p : vecPoints)     //Generate Single Element Circuits
    {
        set<point> singleElementSet;
        singleElementSet.insert(p);
        circuits.push_back(singleElementSet);
    }

    int counter = 0;
    cout << "Queue size " << dxQ.size() << endl;
    while(!dxQ.empty())     //Get next distance/points from queue
    {
        dxPoint dxP = dxQ.front();
        dxQ.pop();

        // 1. Find the circuits (components) for both points
        int p1_idx = isInCircuit(dxP.p1);
        int p2_idx = isInCircuit(dxP.p2);


        if (p1_idx == -1 && p2_idx == -1)
        {
            //P1 not connected AND P2 not connected -> Connect, new circuit
            set<point> newCircuit;

            newCircuit.insert(dxP.p1);
            newCircuit.insert(dxP.p2);
            circuits.push_back(newCircuit);

            counter++;
        }
        else if (p1_idx != -1 && p2_idx == -1)
        {
            //P1 connected to a circuit, P2 not connected -> Add P2 to P1:s circuit
            circuits[p1_idx].insert(dxP.p2);
            counter++;
        }
        else if (p1_idx == -1 && p2_idx != -1)
        {
            //P2 connected to a circuit, P1 not connected -> Add P1 to P2:s circuit
            circuits[p2_idx].insert(dxP.p1);
            counter++;
        }
        else if (p1_idx == p2_idx)
        {
            //P1 AND P2 connected to the same circle -> Gå vidare med livet. Lol!
            continue;
        }
        else //Annars, då...?!?
        {
            //P1 is connected to one circuit, P2 is connected to another circuit -> Merge OwO
            int p_small = min(p1_idx, p2_idx);
            int p_large = max(p1_idx, p2_idx);

            circuits[p_small].insert(circuits[p_large].begin(), circuits[p_large].end());
            circuits.erase(circuits.begin() + p_large); //There can be only one! Mwuahahaha!
            counter++;
        }
    }

    sort(circuits.begin(), circuits.end(), [](set<point> a, set<point> b){
                                            return a.size() > b.size();
                                            });

    cout << "\n 3 Largest Circuit Sizes\n";
    for(size_t i = 0; i < min((size_t)3, circuits.size()); ++i)
    {
         cout << "Circuit " << i + 1 << ": " << circuits[i].size() << " points\n";
    }
    return;
}

int main()
{

    fstream f("input.txt");

    if(!f)
        cout << "Problem med fil " << endl;

    string line;
    int countFreshIds = 0;

    while(getline(f, line))
    {
        vector<string> splitLines = split(line, ",");
        point p;

        p.x = stoll(splitLines[0]);
        p.y = stoll(splitLines[1]);
        p.z = stoll(splitLines[2]);

        vecPoints.push_back(p);
    }
    f.close();


    sort(vecPoints.begin(), vecPoints.end());
    vecPoints.erase(unique(vecPoints.begin(), vecPoints.end()), vecPoints.end());

/*
 *     Loop through points
 *     0 |  1  |  2  |  3  |  4  | -> j
 *     1 | _-__
 *     2 | 1,2 | _-__
 *     3 | 1,3   2,3 |__-__
 *     4 | 1,4   2,4  3,4  | _-__
 *     -> i
 */

    //Loop through point combinations to get distance
    for(int i = 0; i < vecPoints.size(); i++)
    {
        for(int j = i + 1; j < vecPoints.size(); j++)
        {
            long long dist = getDistance(vecPoints[i], vecPoints[j]);

            vecDistances.push_back({vecPoints[i], vecPoints[j], dist});
        }
    }
    sort(vecDistances.begin(), vecDistances.end(), cmpDistances);   //Sort after distances

    partOne();
    partTwo();

    cout << "Hello world!" << endl;
    return 0;
}

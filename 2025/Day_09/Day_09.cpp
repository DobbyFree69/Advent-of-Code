#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#define MAX_PART_1 1000
#define SLOT_LOWER_Y 48785
#define SLOT_UPPER_Y 50003

using namespace std;

typedef pair<long long, long long> pt;
typedef vector<pt> vPt;

vPt redTiles;

struct rect
{
    pt a;
    pt b;
    pt c;
    pt d;
    long long area;
};

typedef vector<rect> vRect;
vector<rect> biggestRects;

void getRectanglePoints(rect& rectangle)
{
    rectangle.c = {rectangle.a.first, rectangle.b.second}; //(x1, y2)
    rectangle.d = {rectangle.b.first, rectangle.a.second}; // (x2, y1)
}

vector<string> split(string str, string delimiter)
{
    vector<string> v;
    if (!str.empty())
    {
        int start = 0;
        do
        {
            // Find the index of occurrence
            int idx = str.find(delimiter, start);
            if (idx == string::npos)
                break;

            // If found add the substring till that
            // occurrence in the vector
            int length = idx - start;
            v.push_back(str.substr(start, length));
            start += (length + delimiter.size());
        }
        while (true);
        v.push_back(str.substr(start));
    }

    return v;
}

bool isPointInPolygon(const vector<pt>& polygon, const pt& point)
{
    typedef pt Point;

    long long n = polygon.size();
    bool inside = false;

    long long x_test = point.first;
    long long y_test = point.second;

    for (int i = 0; i < n; i++)
    {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];

        long long x1 = p1.first;
        long long y1 = p1.second;
        long long x2 = p2.first;
        long long y2 = p2.second;

        bool yCheck = (y1 > y_test) != (y2 > y_test);

        if (y1 == y2 && y1 == y_test)
        {
            if (x_test >= min(x1, x2) && x_test <= max(x1, x2))
                return true;
        }

        if (yCheck)
        {
            long long dy = y2 - y1;
            long long num_right = (x2 - x1) * (y_test - y1);
            long long num_left = (x_test - x1) * dy;

            bool point_is_left_of_edge = false;

            if (dy > 0)
                point_is_left_of_edge = (num_left < num_right);
            else
                point_is_left_of_edge = (num_left > num_right);

            if (point_is_left_of_edge)
                inside = !inside;
        }
    }

    return inside;
}

long long getDistance(pt a, pt b)
{
    long long dx = a.first - b.first;
    long long dy = a.second - b.second;

    return dx * dx + dy * dy;
}

long long getArea(pt a, pt b)
{
    long long dxBX = abs(a.first - b.first) + 1;
    long long dxAY = abs(a.second - b.second) + 1;

    long long area = dxBX * dxAY;

    return area;
}

void printPoint(const pair<long long, long long>& p)
{
    cout << "(" << p.first << "," << p.second << ") ";
}

bool isPointOverForbiddenSlot(pt p)
{
    if (p.second < SLOT_UPPER_Y)
        return false;

    return true;
}

bool isPointUnderForbiddenSlot(pt p)
{
    if (p.second > SLOT_LOWER_Y)
        return false;

    return true;
}


bool checkRectangle(rect rectangle)
{
    //Is rectangle within bounds (redTiles)?
    if (!isPointInPolygon(redTiles, rectangle.a))
        return false;
    if (!isPointInPolygon(redTiles, rectangle.b))
        return false;
    if (!isPointInPolygon(redTiles, rectangle.c))
        return false;
    if (!isPointInPolygon(redTiles, rectangle.d))
        return false;

    //Check upper side of the forbidden area
    bool overForbiddenArea = true;

    if (!isPointOverForbiddenSlot(rectangle.a))
        overForbiddenArea = false;
    if (!isPointOverForbiddenSlot(rectangle.b))
        overForbiddenArea = false;
    if (!isPointOverForbiddenSlot(rectangle.c))
        overForbiddenArea = false;
    if (!isPointOverForbiddenSlot(rectangle.d))
        overForbiddenArea = false;

    if (overForbiddenArea)
        return true;

    //Check lower side of the forbidden area
    bool underForbiddenArea = true;

    if (!isPointUnderForbiddenSlot(rectangle.a))
        underForbiddenArea = false;
    if (!isPointUnderForbiddenSlot(rectangle.b))
        underForbiddenArea = false;
    if (!isPointUnderForbiddenSlot(rectangle.c))
        underForbiddenArea = false;
    if (!isPointUnderForbiddenSlot(rectangle.d))
        underForbiddenArea = false;

    if (underForbiddenArea)
        return true;

    return false;
}

int main()
{
    long long partOneRes;
    long long partTwoRes;

    fstream f("input.txt");

    if (!f)
        cout << "Problem med fil " << endl;

    string line;
    //Parse input into redTiles
    while (getline(f, line))
    {
        vector<string> splitLines = split(line, ",");

        pt tile;
        tile.first = stoll(splitLines[0]);
        tile.second = stoll(splitLines[1]);

        redTiles.push_back(tile);
    }

    f.close();

    //PART ONE
    long long biggestArea = 0;

    pair<pt, pt> biggestOppositeTiles = {{0, 0}, {0, 0}};

    for (int i = 0; i < redTiles.size(); i++)
    {
        for (int j = i + 1; j < redTiles.size(); j++)
        {
            long long currentArea = getArea(redTiles[i], redTiles[j]);

            rect bigRect;

            bigRect.a = redTiles[i];
            bigRect.b = redTiles[j];
            getRectanglePoints(bigRect);

            bigRect.area = currentArea;
            biggestRects.push_back(bigRect);

            if (currentArea > biggestArea)
            {
                biggestArea = currentArea;
                biggestOppositeTiles = make_pair(redTiles[i], redTiles[j]);
            }
        }
    }

    partOneRes = biggestArea;

    //PART TWO

    //Sortera störst area först
    sort(biggestRects.begin(), biggestRects.end(), [](rect rA, rect rB)
    {
        return rA.area > rB.area; // this sorts in ascending order
    });

    for (auto rectangle : biggestRects)
    {
        if (checkRectangle(rectangle))
        {
            partTwoRes = rectangle.area;
            break;
        }
    }

    cout << "Part 1: " << partOneRes << endl;
    cout << "Part 2: " << partTwoRes << endl;

    return 0;
}

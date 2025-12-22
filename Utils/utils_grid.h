
#ifndef TESTPROJEKT_TEST_H
#define TESTPROJEKT_TEST_H


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <functional>
#include <numeric>


void testCout(int x);


template <typename T>
T returnBiggest(T a, T b)
{
    return (a > b) ? a : b;
}

template <typename T>
void printVec(const std::vector<T>& vec, const std::string& label = "", const std::string& sep = " ")
{
    if (!label.empty()) std::cout << label << ": ";

    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i] << (i == vec.size() - 1 ? "" : sep);
    }
    std::cout << std::endl;
}

template <typename T, typename Predicate>
std::vector<T> filter(const std::vector<T>& vec, Predicate pred)
{
    std::vector<T> result;
    // std::copy_if is the standard library way to do this
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(result), pred);
    return result;
}

template <typename T>
bool contains(const std::vector<T>& vec, const T& value)
{
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

template <typename T>
int indexOf(const std::vector<T>& vec, const T& value)
{
    auto it = std::find(vec.begin(), vec.end(), value);
    if (it == vec.end()) return -1;
    return std::distance(vec.begin(), it);
}

template <typename T>
std::vector<T> getUnique(std::vector<T> vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    return vec;
}

template <typename T>
T sumUp(const std::vector<T>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(0));
}

template <typename T>
T multiplyAll(const std::vector<T>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(1), std::multiplies<T>());
}


struct pt3d
{
    long long x;
    long long y;
    long long z;

    bool operator<(const pt3d& other) const
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

    bool operator==(const pt3d& other) const
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }

    bool operator!=(const pt3d& other) const
    {
        return !(*this == other);
    }
};


struct dxPt3d
{
    pt3d p1;
    pt3d p2;
    long long d;

    bool operator <(dxPt3d rhs) const
    {
        return d < rhs.d;
    }
};


struct pos
{
    int x;
    int y;

    bool operator==(const pos& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const pos& other) const
    {
        return !(*this == other);
    }

    bool operator <(pos rhs) const
    {
        if (y == rhs.y)
            return x < rhs.x;

        return y < rhs.y;
    }


    // Manhattan distance is extremely common in AoC
    int manhattan(const pos& other) const
    {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    void printPos(std::string strToAppend = "\n")
    {
        std::cout << "x, y:  " << x << ", " << y << " " << strToAppend;
    }


    pos movePos(int direction, int steps = 1)
    {
        pos rPos = {x, y};

        switch (direction)
        {
        //up
        case 0:
        case '^':
            rPos.y -= steps;
            break;

        //right
        case 1:
        case '>':
            rPos.x += steps;
            break;

        //down
        case 2:
        case 'v':
            rPos.y += steps;
            break;

        //left
        case 3:
        case '<':
            rPos.x -= steps;
            break;

        //do not move as default
        default:
            std::cout << "Error in moving function -> default." << std::endl;
            break;
        }
        return rPos;
    }

    pos diffPos(const pos& b)
    {
        pos diffPos;

        diffPos.y = y - b.y;
        diffPos.x = x - b.x;

        return diffPos;
    }

    pos addPos(const pos& posToAdd)
    {
        pos rPos = {x, y};
        rPos.x += posToAdd.x;
        rPos.y += posToAdd.y;
        return rPos;
    }
};

inline std::ostream& operator<<(std::ostream& os, const pos& p) //Overloading << operator -> pos
{
    return os << "(" << p.x << "," << p.y << ")";
}

//POS: Function declarations
pos movePos(pos startP, int direction, int steps = 1);
pos diffPos(const pos& a, const pos& b);
int turnDir(int currentDirection, int steps);


struct Grid
{
    std::vector<std::string> data;
    int rows, cols;

    // The index cache - MAP each char in the grid to a vector of its positions
    std::map<char, std::vector<pos>> charPositionsMap;

    Grid(std::vector<std::string> input) : data(input)
    {
        rows = input.size();
        cols = input.empty() ? 0 : input[0].size();
    }

    // Safe access
    bool isInRange(pos p) const
    {
        return p.x >= 0 && p.x < cols && p.y >= 0 && p.y < rows;
    }

    char at(pos p) const
    {
        if (!isInRange(p))
            return -1; // Return a wall/null if out of bounds
        return data[p.y][p.x];
    }

    void set(pos p, char c)
    {
        if (isInRange(p))
            data[p.y][p.x] = c;
    }

    void printGrid(const std::set<pos>& overlay = {}, char overlayChar = 'X') const
    {
        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                if (overlay.count({x, y}))
                {
                    std::cout << overlayChar;
                }
                else
                {
                    std::cout << data[y][x];
                }
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }


    //Hashing of Grid state in Single String -> Set, Map !FOR MEMOIZATION!
    std::string getGridHashAsStr() const
    {
        std::string hash = "";
        for (const auto& row : data)
            hash += row;
        return hash;
    }

    // Counts distinct characters and their frequencies
    std::map<char, int> countDistinctChars() const
    {
        std::map<char, int> counts;
        for (const auto& row : data)
        {
            for (char c : row)
            {
                counts[c]++;
            }
        }
        return counts;
    }

    void printDistinctChars()
    {
        std::map<char, int> rDistinctCharMap = countDistinctChars();

        std::cout << "Distinct chars \n";
        for (auto key : rDistinctCharMap)
        {
            std::cout << key.first << " " << key.second << std::endl;
        }
    }

    void printRange()
    {
        std::cout << "Grid Max Range:\nRows: " << rows << " Cols: " << cols << std::endl;
    }

    template <typename F>
    void forEach(F func)
    {
        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                // Pass the position and the character at that position to the function
                func(pos{x, y}, data[y][x]);
            }
        }
    }


    //Returns all positions of a specific character
    std::vector<pos> findAllChPos(char target) const
    {
        std::vector<pos> positions;
        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                if (data[y][x] == target)
                {
                    positions.push_back({x, y});
                }
            }
        }
        return positions;
    }

    // Returns just the first position found (useful for 'S' or 'E' markers)
    // Returns {-1, -1} if not found
    pos findFirstChPos(char target) const
    {
        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                if (data[y][x] == target)
                    return {x, y};
            }
        }
        return {-1, -1};
    }

    //MAPPING EACH CHARS TO A VECTOR OF ITS POSITIONS
    // This function populates the charPositionsMap
    void buildCharPositionsMap()
    {
        charPositionsMap.clear();
        for (int y = 0; y < rows; ++y)
        {
            for (int x = 0; x < cols; ++x)
            {
                char c = data[y][x];
                charPositionsMap[c].push_back({x, y});
            }
        }
    }


    // 1. Get 4-way neighbors with optional distance multiplier
    std::vector<pos> getAdjacentPos4(pos p, int dist = 1)
    {
        std::vector<pos> neighbors;
        // Basic unit offsets
        static const std::vector<pos> units = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

        for (const auto& u : units)
        {
            // Apply the multiplier to the unit offset
            pos off = {u.x * dist, u.y * dist};
            pos next = p.addPos(off);

            if (isInRange(next))
            {
                neighbors.push_back(next);
            }
        }
        return neighbors;
    }

    // 2. Get 8-way neighbors with optional distance multiplier
    std::vector<pos> getAdjacentPos8(pos p, int dist = 1)
    {
        std::vector<pos> neighbors;
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                if (dx == 0 && dy == 0) continue;

                // Apply the multiplier to the loop offsets
                pos off = {dx * dist, dy * dist};
                pos next = p.addPos(off);

                if (isInRange(next))
                {
                    neighbors.push_back(next);
                }
            }
        }
        return neighbors;
    }


    //Get a single ray of positions in a specific direction
    // Returns all positions from p (exclusive) until the grid boundary
    std::vector<pos> getRayLine(pos p, pos direction)
    {
        std::vector<pos> ray;
        pos current = p.addPos(direction);

        while (isInRange(current))
        {
            ray.push_back(current);
            current = current.addPos(direction);
        }
        return ray;
    }

    //Get all 4 cardinal rays/lines (Up, Right, Down, Left)
    // Useful for "Can this tower see the edge?" or "Is this tree visible?"
    std::vector<std::vector<pos>> getRayLines4Directions(pos p)
    {
        std::vector<std::vector<pos>> rays;
        static const std::vector<pos> units = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

        for (const auto& u : units)
        {
            rays.push_back(getRayLine(p, u));
        }
        return rays;
    }

    // 3. Find the first obstacle in a direction
    // Returns the position of the first char that matches 'target'
    pos findFirstObstacleOnRayLine(pos p, pos direction, char obstacle = '#')
    {
        pos current = p.addPos(direction);
        while (isInRange(current))
        {
            if (at(current) == obstacle)
                return current;
            current = current.addPos(direction);
        }
        return {-1, -1}; // Not found
    }


    struct Iterator
    {
        int x, y, cols;
        using iterator_category = std::forward_iterator_tag;
        using value_type = pos;
        using difference_type = std::ptrdiff_t;
        using pointer = pos*;
        using reference = pos&;

        // This allows the iterator to return a 'pos' when dereferenced
        pos operator*() const { return {x, y}; }

        // Prefix increment: moves to next cell, wrapping at the end of rows
        Iterator& operator++()
        {
            if (++x >= cols)
            {
                x = 0;
                ++y;
            }
            return *this;
        }

        // Comparison for the loop condition
        bool operator!=(const Iterator& other) const
        {
            return x != other.x || y != other.y;
        }
    };

    Iterator begin() const { return {0, 0, cols}; }
    Iterator end() const { return {0, rows, cols}; } // Points just past the last row
};


Grid readGrid(const std::string& filename);
void printVecPos(std::vector<pos>& vecPositions);

#endif //TESTPROJEKT_TEST_H

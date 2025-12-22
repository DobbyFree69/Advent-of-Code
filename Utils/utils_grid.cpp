
#include "utils_grid.h"



void testCout(int x)
{
    std::cout << "Test Cout " << x << std::endl;
    return;
}



pos movePos(pos startP, int direction, int steps)
{
    pos rpos;

    switch(direction)
    {
        //up
    case 0: case '^':
        rpos.x = startP.x;
        rpos.y = startP.y - steps;
        break;
    //right
    case 1: case '>':
        rpos.x = startP.x + steps;
        rpos.y = startP.y;
        break;

        //down
    case 2: case 'v':
        rpos.x = startP.x;
        rpos.y = startP.y + steps;
        break;


        //left
    case 3: case '<':
        rpos.x = startP.x - steps;
        rpos.y = startP.y;
        break;

        //do not move as default
    default:
        std::cout << "Error in moving function -> default." << std::endl;
        rpos = startP;
        break;
    }
    return rpos;
}


pos diffPos(pos a, pos b)
{
    pos diffPos;

    diffPos.y = a.y - b.y;
    diffPos.x = a.x - b.x;

    return diffPos;
}

pos diffPos(const pos& a, const pos& b)
{
    pos diffPos;

    diffPos.y = a.y - b.y;
    diffPos.x = a.x - b.x;

    return diffPos;
}

int turnDir(int currentDirection, int steps) //steps: 1 -> right turn, -1 -> left (90 degrees)
{
    return (currentDirection + steps + 4) % 4;
}

Grid readGrid(const std::string& filename)
{
    std::ifstream file("input.txt");
    if (!file)
        std::cout << "You fucked it up! Input file! \n";

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            lines.push_back(line);
    }

    file.close();
    return Grid(lines);
}


void printVecPos(std::vector<pos>& vecPositions)
{
    for (auto& position : vecPositions)
        position.printPos();
}




#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

#define MAX 140

//Accessible rolls: if there are fewer than four rolls of paper

using namespace std;

char grid[MAX][MAX];


bool checkAndGetRolls(pair<int, int> posXY, vector<pair<int, int>>& adjecent)
{
    int countAdjecentRolls = 0;

    for(int i_x = posXY.first-1; i_x <= posXY.first+1; i_x++)
    {
        //Out of bounds
        if((i_x < 0) || (i_x > 139))
            continue;

        for(int j_y = posXY.second-1; j_y <= posXY.second+1;j_y++)
        {
            pair<int, int> tempPos = {i_x, j_y};

            if((j_y < 0) || (j_y > 139)|| (posXY == tempPos))
                continue;

            if(grid[j_y][i_x] == '@')           //Confusing, right? y, x Hehe...
            {
                countAdjecentRolls++;
                adjecent.push_back(tempPos);
            }

            if(countAdjecentRolls >= 4)
                return false;
        }
    }

    if(countAdjecentRolls < 4)                     //check if roll is accessible
    {
        //grid[posXY.second][posXY.first] = 'X';     //Ac
        return true;
    }
    return false;
}

int countRemovedRollsPart2(vector<pair<int, int>> positions)
{
    vector<pair<int, int>> positionsToCheckNext;

    int removedRolls = 0;

    for(pair<int, int> position : positions)
    {
        if(grid[position.second][position.first] != '@')
            continue;

        if(!checkAndGetRolls(position, positionsToCheckNext))
        {
            continue;
        }
        else //if(checkAndGetRolls(position, adjecentRolls)) //ÄNDRA till rolls removed
        {
            removedRolls++;
            grid[position.second][position.first] = 'X';    //mark position as removed
        }
    }

    //Base Case: inga rullar kvar att ta bort -> fork lift certified helpers OwO
    if(removedRolls == 0)
        return removedRolls;

    if(!positionsToCheckNext.empty())
    {
        //Ta bort dubletter ur positionsToCheckNext
        sort(positionsToCheckNext.begin(), positionsToCheckNext.end());
        auto last = unique(positionsToCheckNext.begin(), positionsToCheckNext.end());
        positionsToCheckNext.erase(last, positionsToCheckNext.end());

        //Rekursion --> UUUUUiiiiii
        removedRolls += countRemovedRollsPart2(positionsToCheckNext);
       // cout << " removedRolls " << removedRolls << " adjecentsize " << positionsToCheckNext.size() << endl;
    }

  return removedRolls;
}

bool checkRollAccessible(pair<int, int> posXY)
{

    int countAdjecentRolls = 0;
    for(int i_x = posXY.first-1; i_x <= posXY.first+1; i_x++)
    {
        if((i_x < 0) || (i_x >= 140))
            continue;

        for(int j_y = posXY.second-1; j_y <= posXY.second+1;j_y++)
        {
            pair<int, int> tempPos = {i_x, j_y};
            if((j_y < 0) || (j_y >= 140)|| (posXY == tempPos))
                continue;

            if(grid[j_y][i_x] == '@')
                countAdjecentRolls++;

            if(countAdjecentRolls >= 4)
                return false;
        }
    }

    return (countAdjecentRolls < 4) ? true : false;

}

int countRolls(vector<pair<int, int>> positions)
{
    int accessibleRolls = 0;

    for(pair<int, int> position : positions)
    {
        if(checkRollAccessible(position))
            accessibleRolls++;
    }

    return accessibleRolls;
}

int main()
{


    vector<pair<int, int>> paperPositions;

    ifstream f("input.txt");
    if(!f)
        cout << "You fucked it up!" << endl;

    char ch;
    int counteruniquePositions = 0;
    int i = 0, j = 0;


    do
    {
        f.get(ch);
        grid[i][j] = ch;

        if((ch != '.') && (ch != '\n'))
        {
            //x = j; y = i -> x, y
            pair<int, int> aktuellPos = {j, i};


            if(ch == '@')
                paperPositions.push_back(aktuellPos);
        }

        i += (ch == '\n') ? 1 : 0;
        j =  (ch == '\n') ? 0 : j+1;

    }while(!f.eof());
    f.close();

    //cout << "paperPositions size " << paperPositions.size() << endl;

    cout << "Accessible Rolls  PART 1: " << countRolls(paperPositions) << endl;

    cout << "Removed Rolls     PART 2:" << countRemovedRollsPart2(paperPositions) << endl;

    //printMap();

    cout << "Hello world!" << endl;
    return 0;
}

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


vector<vector<int>> numbers;
vector<vector<string>> vStrNumbersCol;
vector<string> lines;
string opsline;
vector<char> ops;

vector<int> getNumbers(int col)
{
    vector<int> iNewNumbers;


    if (vStrNumbersCol[col].empty())
        return iNewNumbers;

    // Find the longest string
    size_t maxLen = 0;
    for (string& s : vStrNumbersCol[col])
        maxLen = max(maxLen, s.length());

    // For each length -> get chars on that vertical line
    for (int i = (int)maxLen; i > 0; i--)
    {
        string sCurrentNewNumber = "";

        for (string& strCurrent : vStrNumbersCol[col])
        {
            //If there is a char at this position -> add too sCurrentNewNumber
            if (strCurrent.length() >= i)
            {
                if (strCurrent[i - 1] == '0' || strCurrent[i - 1] == ' ')
                    continue;

                sCurrentNewNumber.push_back(strCurrent[i - 1]);
            }
        }

        // Convert sCurrentNewNumber to integer
        if (!sCurrentNewNumber.empty())
        {
            stringstream ss(sCurrentNewNumber);
            int iNewNumber;
            if (ss >> iNewNumber)
                iNewNumbers.push_back(iNewNumber);
        }
    }
    return iNewNumbers;
}

long long partTwo()
{
    long long totalSumPart2 = 0;

    for(auto opCol = 0; opCol < ops.size() ; opCol++)
    {
        vector<int> numbersPartTwo;
        numbersPartTwo = getNumbers(opCol);

        if (ops[opCol] == '+')
        {
            long long sumColP2 = 0;
            for (auto currentRow : numbersPartTwo)
                sumColP2 += currentRow;

            totalSumPart2 += sumColP2;
        }
        else if (ops[opCol] == '*')
        {
            long long prodColP2 = 1;

            for (auto currentRow : numbersPartTwo)
            {
                if (!numbersPartTwo.empty())
                    prodColP2 *= currentRow;
            }
            totalSumPart2 += prodColP2;
        }
    }
    return totalSumPart2;
}

long long partOne()
{
    long long totalSum = 0;

    for(auto opCol = 0; opCol < ops.size(); opCol++)
    {
        if (ops[opCol] == '+')
        {
            long long sumCol = 0;

            for(auto i = 0; i < numbers.size(); i++)
                sumCol+= numbers[i][opCol];

            totalSum += sumCol;
        }
        else if (ops[opCol] == '*')
        {
            long long prodCol = 1;

            for(auto j = 0; j < numbers.size(); j++)
                prodCol *= numbers[j][opCol];

            totalSum += prodCol;
        }
    }
    return totalSum;
}


void parseCols()
{
    int currentLength = 0;

    //parse globalOpsLine back to front
    for (int i = opsline.size()-1; i >= 0; i--)
    {
        if (opsline[i] == ' ')
            currentLength++;

        if (opsline[i] == '+' || opsline[i] == '*')
        {
            ops.push_back(opsline[i]);
            currentLength++;


            vector<string> currentCol;
            for (auto rowLine: lines) //for each row in lines get cols (substring)
            {
                string subStrCol = rowLine.substr(i, currentLength);
                currentCol.push_back(subStrCol);
            }
            vStrNumbersCol.push_back(currentCol);

            currentLength = -1;
        }
    }
}

int main()
{
    fstream fops("input_operators.txt");   //Read operator file

    if(!fops)
    {
        cout << "You fucked it up! Operators" << endl;
        return 0;
    }

    getline(fops, opsline);
    fops.close();

    fstream fnumbers("input.txt");

    if(!fnumbers)
    {
        cout << "You fucked it up! " << endl;
        return 0;
    }

    string line;
    while(getline(fnumbers, line))
    {
        lines.push_back(line);

        stringstream ss(line);      //PART ONE Parsing
        vector<int> rowNumbers;
        int number;

        while (ss >> number)
            rowNumbers.push_back(number);

        reverse(rowNumbers.begin(), rowNumbers.end());  //Parsing Operators for P2
        numbers.push_back(rowNumbers);                          //back to front
    }
    fnumbers.close();

    parseCols();

    long long p1 = partOne();
    long long p2 = partTwo();

    cout << "Part 1: " << p1 << endl;
    cout << "Part 2: " << p2 << endl;

    cout << "Hello world!" << endl;
    return 0;
}

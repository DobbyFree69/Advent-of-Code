#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;



vector<string> batteryBlocks;

vector<int> convertStringToDigits(const string& digitString)          //Varför gjorde jag så här?!?
{
    vector<int> result;
    // Iterate over each character in the input string
    for (char c : digitString)
    {
        // Check if the character is a digit before converting
        if (isdigit(c))
        {
            result.push_back(c - '0');
        }
    }
    return result;
}



string digitsToString(int digitA, int digitB)                       //Nämen, det var väl ändå onödigt!
{
    string rString = to_string(digitA);
    rString += to_string(digitB);

    return rString;
}


string removeDubbla(string s)                                       //Such stuff...
{
    string r = "";

    for(auto i = 0; i < s.length(); i++)
    {
        r += s[i];

        for(auto j = 1; j <= s.length(); j++)
        {
            if(s[i] != s[i+j])
            {
                i += j-1;
                break;
            }
        }
    }

    return r;
}




//PART 2
void recPart_two(string block, long long& highest, int index, string strTemp)
{

    //base Case

    if(strTemp.length() == 12)
    {
        long long joltage = stoll(strTemp);

        if(joltage > highest)
            highest = joltage;

        return;
    }

    char biggestLocal = 0;
    size_t biggestLocalIndex = index;

    for(auto i = index; i <= block.length() - (12 - strTemp.length()); i++)
    {

        if(block[i] > biggestLocal)
        {
            biggestLocal = block[i];
            biggestLocalIndex = i;
        }
 //      BRUTE FORCE: Funkar inte :/
 //     recPart_two(block, highest, i+1, strTemp + block[i]);
    }

    recPart_two(block, highest, biggestLocalIndex+1, strTemp + block[biggestLocalIndex]);

    return;
}


long long part_two(string block)
{

    long long highestHighest = 0;
    char bigDigit = block[0];

    recPart_two(block, highestHighest, 0, "");

    return highestHighest;
}


//PART 1
int getHighestJoltage(string block)
{

    int highest = 0;

    for(auto i = 0; i < block.length()-1; i++)
    {
        string strTemp = {block[i]};

        for(auto j = i+1; j < block.length(); j++)
        {
            strTemp += block[j];
            int joltage = stoi(strTemp);

            if(joltage > highest)
                highest = joltage;

            strTemp.pop_back();
        }
    }

    return highest;
}

int main()
{
    auto begin = std::chrono::high_resolution_clock::now();

    string filename = "input.txt";

    ifstream inputFile(filename);

    if (!inputFile)
        cout << "Problem med filen" << endl;

    string line;

    while(getline(inputFile, line))
        batteryBlocks.push_back(line);

    inputFile.close();

    //PART 1
    int sumHighestJoltages = 0;

    for(auto block: batteryBlocks)
        sumHighestJoltages += getHighestJoltage(block);



    //PART 2
    long long sumHighestJoltages_DEL2 = 0;
   // sumHighestJoltages_DEL2 += part_two("5634214554421238425624233425223147354453753545362612224266323235522445445564713223426114342124645344");


    for(auto block: batteryBlocks)
        sumHighestJoltages_DEL2 += part_two(block);

    cout << "Sum PART 1: " << sumHighestJoltages << endl;
    cout << "Sum PART 2: " << sumHighestJoltages_DEL2 << endl;

    // Stop measuring time and calculate the elapsed time
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);

    return 0;
}

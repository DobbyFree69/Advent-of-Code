#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


vector<long long> upperLimits;
vector<long long> lowerLimits;
vector<long long> invalidIds;

void checkStrId(string strId)
{
    int strIdLength = strId.length();

    for(int i = 3; i <= strIdLength; i++)
    {
        if ((strIdLength % i) != 0)                 //kolla om delbart
            continue;

        int partialStringLen = strIdLength/i;

        string subStr = strId.substr(0, partialStringLen);

        bool checkPartialStr = true;

        for(int partialString = 1; partialString < i; partialString++)                      //kolla för varje delstring om de fortfarande
        {
            string strB = strId.substr(partialString*partialStringLen, partialStringLen);   //är en upprepning av subStr

           if(strB != subStr)
           {
               checkPartialStr = false;
               break;
           }
        }
        if(checkPartialStr)
        {
            long long invalidId = stoll(strId);
            invalidIds.push_back(invalidId);
        }
    }
}

void generateIDs()
{
        for (auto limit = 0; limit <= upperLimits.size(); limit++) //for each upper/lower bound limit:
        {
            for(long long currentId = lowerLimits[limit]; currentId <= upperLimits[limit]; currentId++)
            {
                string strId = to_string(currentId);
                int strIdLength = strId.length();

                //Funktion för DEL 2
                checkStrId(strId);

                //DEL 1
                if((strIdLength % 2) != 0)              //Kolla om stringlängden kan delas jämnt i hälften
                    continue;

                string strA = strId.substr(0, strIdLength/2);
                string strB = strId.substr(strIdLength/2);
                if(strA.compare(strB) == 0)
                {
                    long long invalidId = stoll(strId);
                    invalidIds.push_back(invalidId);
                }
            }
        }
}



void parseInput(string input)
{

    stringstream ss(input);
    string tempStrLimits;



    while(getline(ss, tempStrLimits, ','))
    {
        string strUpperLimit;
        string strLowerLimit;
        string delLimits = "-";

        auto pos = tempStrLimits.find(delLimits);
        strLowerLimit = tempStrLimits.substr(0, pos);
        tempStrLimits.erase(0, pos + delLimits.length());
        //upperLimit = tempStrLimits.substr(0, npos);
        strUpperLimit = tempStrLimits;

        long long lUpperLimit = stoll(strUpperLimit);
        long long lLowerLimit = stoll(strLowerLimit);


        upperLimits.push_back(lUpperLimit);
        lowerLimits.push_back(lLowerLimit);

    }
}









int main()
{
    string input;

    fstream file("input.txt");

    if(!file)
        cout << "Problem med fil " << endl;


    do
    {
        getline(file, input);
        cout << input << endl;
    }while(!file.eof());

    file.close();

    parseInput(input);
    generateIDs();


    long long sumInvalidIds = 0;

    //hantera vector-element: ta bort dubletter
    sort(invalidIds.begin(), invalidIds.end());
    auto it = unique(invalidIds.begin(), invalidIds.end());
    invalidIds.erase(it, invalidIds.end());
    sort(invalidIds.begin(), invalidIds.end());


    //summera upp invalidIDs
    for(auto invalidID : invalidIds)
    {
        sumInvalidIds += invalidID;
        cout << "Invalid ID " << invalidID << " Sum " << sumInvalidIds << endl;
    }




    cout << "Hello world!" << endl;
    return 0;
}
